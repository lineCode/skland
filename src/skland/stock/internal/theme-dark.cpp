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

#include "theme-dark.hpp"
#include "theme-default.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/abstract-shell-view.hpp>
#include <skland/gui/label.hpp>
#include <skland/gui/context.hpp>

#include <skland/graphic/gradient-shader.hpp>

namespace skland {

/**
 * @brief The default window frame
 */
class WindowFrameDark final : public WindowFrameDefault {

  WindowFrameDark(const WindowFrameDark &orig) = delete;
  WindowFrameDark &operator=(const WindowFrameDark &other) = delete;

 public:

  WindowFrameDark()
      : WindowFrameDefault() {
    TitleBar *title_bar = static_cast<TitleBar *>(this->GetTitleView());
    title_bar->SetForeground(0xFF999999);
    title_bar->close_button()->SetForeground(0xFF999999);
    title_bar->close_button()->SetBackground(0xFF444444);
    title_bar->maximize_button()->SetForeground(0xFF999999);
    title_bar->maximize_button()->SetBackground(0xFF444444);
    title_bar->minimize_button()->SetForeground(0xFF999999);
    title_bar->minimize_button()->SetBackground(0xFF444444);
  }

  virtual ~WindowFrameDark() {}

 protected:

  virtual void OnDraw(const Context *context);

};

void WindowFrameDark::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f, shell_view()->GetSize().width, shell_view()->GetSize().height);

  // Drop shadow:
  if ((!shell_view()->IsMaximized()) || (!shell_view()->IsFullscreen())) {
    float radii[] = {
        7.f, 7.f, // top-left
        7.f, 7.f, // top-right
        4.f, 4.f, // bottom-right
        4.f, 4.f  // bottom-left
    };
    path.AddRoundRect(geometry, radii);
    canvas->Save();
    canvas->ClipPath(path, kClipDifference, true);
    DrawShadow(canvas.get());
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

  // Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEF202020);
  canvas->DrawPath(path, paint);

  // Draw the client area:
  paint.SetColor(0xEF303030);
  canvas->Save();
  canvas->ClipPath(path, kClipIntersect, true);
  canvas->DrawRect(GetContentGeometry(), paint);
  canvas->Restore();

  canvas->Flush();
}

}

void *WindowFrameDarkCreate() {
  return new skland::WindowFrameDark;
}

void WindowFrameDarkDestroy(void *p) {
  skland::WindowFrameDark *frame = static_cast<skland::WindowFrameDark *>(p);
  delete frame;
}
