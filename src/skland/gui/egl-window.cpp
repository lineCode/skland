/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <skland/gui/egl-window.hpp>

#include <skland/gui/display.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/output.hpp>
#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/sub-surface.hpp>
#include <skland/gui/egl-surface.hpp>

#include <skland/graphic/canvas.hpp>

#include "internal/redraw-task.hpp"

#include <GLES2/gl2.h>

namespace skland {

EGLWindow::EGLWindow(const char *title, AbstractWindowFrame *frame)
    : EGLWindow(400, 300, title, frame) {

}

EGLWindow::EGLWindow(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, frame),
      main_surface_(nullptr),
      sub_surface_(nullptr),
      egl_surface_(nullptr),
      animating_(false) {
  Surface *parent = toplevel_shell_surface()->surface();

  main_surface_ = new SubSurface(parent, this, parent->margin());
  DBG_ASSERT(main_surface_->surface()->parent() == parent);
  DBG_ASSERT(main_surface_->surface()->below() == parent);

  sub_surface_ = new SubSurface(main_surface_->surface(), this);
  DBG_ASSERT(sub_surface_->surface()->parent() == main_surface_->surface());
  DBG_ASSERT(sub_surface_->surface()->below() == main_surface_->surface());

  wayland::Region empty_region;
  empty_region.Setup(Display::wl_compositor());
  main_surface_->surface()->SetInputRegion(empty_region);
  sub_surface_->surface()->SetInputRegion(empty_region);

  Rect client_rect = GetClientGeometry();
  sub_surface_->SetWindowPosition((int) client_rect.l, (int) client_rect.t);
//    sub_surface_->SetDesync();  // FIXME: looks no difference between sync and desync modes

  egl_surface_ = new EGLSurface(sub_surface_->surface());
  egl_surface_->Resize((int) client_rect.width(), (int) client_rect.height());

  frame_callback_.done().Set(this, &EGLWindow::OnFrame);
}

EGLWindow::~EGLWindow() {
  delete egl_surface_;
  delete sub_surface_;
  delete main_surface_;
}

void EGLWindow::OnShown() {
  Surface *shell_surface = toplevel_shell_surface()->surface();

  // Create buffer:
  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();  // The current screen size
  }

  int total_width = std::max(this->width(), output_size.width);
  int total_height = std::max(this->height(), output_size.height);
  total_width += shell_surface->margin().lr();
  total_height += shell_surface->margin().tb();

  frame_pool_.Setup(total_width * 4 * total_height);
  frame_buffer_.Setup(frame_pool_, total_width, total_height,
                      total_width * 4, WL_SHM_FORMAT_ARGB8888);

  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.pixel(),
                                 frame_buffer_.size().width,
                                 frame_buffer_.size().height));
  frame_canvas_->SetOrigin((float) shell_surface->margin().left,
                           (float) shell_surface->margin().top);
  frame_canvas_->Clear();

  main_pool_.Setup(total_width * 4 * total_height);
  main_buffer_.Setup(main_pool_, total_width, total_height,
                     total_width * 4, WL_SHM_FORMAT_ARGB8888);
  main_surface_->surface()->Attach(&main_buffer_);
  main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                main_buffer_.size().width,
                                main_buffer_.size().height));
  main_canvas_->SetOrigin((float) main_surface_->surface()->margin().left,
                          (float) main_surface_->surface()->margin().top);
  main_canvas_->Clear();

  // initialize EGL

  UpdateAll();
}

void EGLWindow::OnUpdate(AbstractView *view) {
  if (!visible()) return;

  Surface *surface = nullptr;

  if (view == this) {
    surface = toplevel_shell_surface()->surface();
    kRedrawTaskTail.PushFront(redraw_task().get());
    redraw_task()->context = Context(surface, frame_canvas_);
    DBG_ASSERT(frame_canvas_);
    Damage(this, 0, 0,
           width() + surface->margin().lr(),
           height() + surface->margin().tb());
    surface->Commit();
  } else {
    std::shared_ptr<Canvas> canvas;
    surface = main_surface_->surface();
    canvas = main_canvas_;

    RedrawTask *task = GetRedrawTask(view);
    kRedrawTaskTail.PushFront(task);
    task->context = Context(surface, canvas);
    DBG_ASSERT(canvas);
    Damage(view, view->x() + surface->margin().left,
           view->y() + surface->margin().top,
           view->width(),
           view->height());
    surface->Commit();
  }
}

Surface *EGLWindow::OnGetSurface(const AbstractView *view) const {
  if (view == this)
    return toplevel_shell_surface()->surface();

  return nullptr != sub_surface_ ? sub_surface_->surface() : toplevel_shell_surface()->surface();
}

void EGLWindow::OnResize(int width, int height) {
  RectI input_rect(width, height);
  Surface *shell_surface = toplevel_shell_surface()->surface();

  input_rect.left = shell_surface->margin().left - AbstractWindowFrame::kResizingMargin.left;
  input_rect.top = shell_surface->margin().top - AbstractWindowFrame::kResizingMargin.top;
  input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                    height + AbstractWindowFrame::kResizingMargin.tb());

  wayland::Region input_region;
  input_region.Setup(Display::wl_compositor());
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  ResizeWindowFrame(window_frame(), width, height);

  // Reset buffer:
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int total_size = width * 4 * height;
  if (total_size > frame_pool_.size()) {
    DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n",
                  total_size, frame_pool_.size(), "Re-generate shm pool");
    frame_pool_.Setup(total_size);
    main_pool_.Setup(total_size);
  }

  frame_buffer_.Setup(frame_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.pixel(),
                                 frame_buffer_.size().width,
                                 frame_buffer_.size().height));
  frame_canvas_->SetOrigin(shell_surface->margin().left, shell_surface->margin().top);
  frame_canvas_->Clear();

  main_buffer_.Setup(main_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  main_surface_->surface()->Attach(&main_buffer_);
  main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                main_buffer_.size().width,
                                main_buffer_.size().height));
  main_canvas_->SetOrigin(main_surface_->surface()->margin().left,
                          main_surface_->surface()->margin().top);
  main_canvas_->Clear();

  Rect client_rect = GetClientGeometry();
  egl_surface_->Resize((int) client_rect.width(), (int) client_rect.height());
  OnResizeEGL(this->width(), this->height());

  UpdateAll();
}

void EGLWindow::OnDraw(const Context *context) {
  if (window_frame()) DrawWindowFrame(window_frame(), context);

  if (!animating_) {
    animating_ = true;
    egl_surface_->surface()->SetupCallback(frame_callback_);
    OnInitializeEGL();
  }
}

void EGLWindow::OnInitializeEGL() {

}

void EGLWindow::OnResizeEGL(int width, int height) {

}

void EGLWindow::OnRenderEGL() {
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

bool EGLWindow::MakeCurrent() {
  return egl_surface_->MakeCurrent();
}

void EGLWindow::SwapBuffers() {
  if (egl_surface_->SwapBuffers())
    egl_surface_->surface()->Commit();
}

void EGLWindow::OnFrame(uint32_t serial) {
  egl_surface_->surface()->SetupCallback(frame_callback_);
  OnRenderEGL();
}

void EGLWindow::OnRelease() {
//  fprintf(stderr, "on release\n");
}

}