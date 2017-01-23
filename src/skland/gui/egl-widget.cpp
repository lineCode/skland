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

#include <skland/gui/egl-widget.hpp>

#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include "internal/redraw-task.hpp"

#include <stdlib.h>
#include <assert.h>

namespace skland {

static const char *vert_shader_text =
    "uniform mat4 rotation;\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "varying vec4 v_color;\n"
        "void main() {\n"
        "  gl_Position = rotation * pos;\n"
        "  v_color = color;\n"
        "}\n";

static const char *frag_shader_text =
    "precision mediump float;\n"
        "varying vec4 v_color;\n"
        "void main() {\n"
        "  gl_FragColor = v_color;\n"
        "}\n";

static GLuint
create_shader(const char *source, GLenum shader_type) {
  GLuint shader;
  GLint status;

  shader = glCreateShader(shader_type);
  assert(shader != 0);

  glShaderSource(shader, 1, (const char **) &source, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetShaderInfoLog(shader, 1000, &len, log);
    fprintf(stderr, "Error: compiling %s: %*s\n",
            shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
            len, log);
    exit(1);
  }

  return shader;
}

EGLWidget::EGLWidget()
    : EGLWidget(400, 300) {
}

EGLWidget::EGLWidget(int width, int height)
    : AbstractWidget(width, height),
      surface_(nullptr) {
  surface_ = new EGLSurface(this);

  // Set empty regions
  empty_opaque_region_.Setup(Display::wl_compositor());
  surface_->SetOpaqueRegion(empty_opaque_region_);
  empty_input_region_.Setup(Display::wl_compositor());
  surface_->SetInputRegion(empty_input_region_);

  frame_callback_.done().Set(this, &EGLWidget::OnFrame);
//  InitializeGL();
}

EGLWidget::~EGLWidget() {
  delete surface_;
}

AbstractSurface *EGLWidget::OnGetSurface(const AbstractView * /* view */) const {
  return surface_;
}

void EGLWidget::OnResize(int width, int height) {
  resize(width, height);
  surface_->Resize(width, height);
  if (!surface_->wl_sub_surface().IsValid()) {
    Update();
  }
}

void EGLWidget::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseLeave(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseButton(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void EGLWidget::OnDraw(const Context *context) {
  fprintf(stderr, "on draw\n");
  if (!surface_->wl_sub_surface().IsValid()) {
    AbstractSurface *parent_surf = context->surface();
    parent_surf->AddSubSurface(surface_);
    surface_->SetPosition(parent_surf->margin().l + (int) geometry().x(),
                          parent_surf->margin().t + (int) geometry().y());
    surface_->SetSync();
    OnInitializeEGL();

//    OnRender();
//    surface_->Damage(0, 0, (int) geometry().width(), (int) geometry().height());
//    surface_->Commit();
  }
}

void EGLWidget::OnInitializeEGL() {
  if (surface_->MakeCurrent()) {
    frame_callback_.Setup(surface_->wl_surface());
    glClearColor(0.1, 0.1, .85, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    surface_->SwapBuffers();
  }
}

void EGLWidget::OnResizeEGL() {
  if (surface_->MakeCurrent()) {
    glClearColor(0.1, 0.1, .85, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    surface_->SwapBuffers();
  }
}

void EGLWidget::OnRender() {
  glClearColor(0.36, 0.85, 0.27, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void EGLWidget::OnFrame(uint32_t /* serial */) {
  static uint64_t count = 0;
  fprintf(stderr, "on render: %ld\n", count);
  count++;

  if (surface_->MakeCurrent()) {
    frame_callback_.Setup(surface_->wl_surface());
    OnRender();
    surface_->SwapBuffers();
  }
}

void EGLWidget::InitializeGL() {
  GLuint frag, vert;
  GLuint program;
  GLint status;

  frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
  vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

  program = glCreateProgram();
  glAttachShader(program, frag);
  glAttachShader(program, vert);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program, 1000, &len, log);
    fprintf(stderr, "Error: linking:\n%*s\n", len, log);
    exit(1);
  }

  glUseProgram(program);

  pos = 0;
  col = 1;

  glBindAttribLocation(program, pos, "pos");
  glBindAttribLocation(program, col, "color");
  glLinkProgram(program);

  rotation_uniform = glGetUniformLocation(program, "rotation");
}

}
