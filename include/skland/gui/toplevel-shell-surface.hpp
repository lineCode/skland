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

#ifndef SKLAND_GUI_TOPLEVEL_SHELL_SURFACE_HPP_
#define SKLAND_GUI_TOPLEVEL_SHELL_SURFACE_HPP_

#include "../core/margin.hpp"
#include "../wayland/xdg-toplevel.hpp"

namespace skland {

class AbstractEventHandler;
class Surface;
class ShellSurface;

/**
 * @ingroup gui
 * @brief Xdg toplevel shell surface
 */
class ToplevelShellSurface {

  friend class ShellSurface;

  ToplevelShellSurface() = delete;
  ToplevelShellSurface(const ToplevelShellSurface &) = delete;
  ToplevelShellSurface &operator=(const ToplevelShellSurface &) = delete;

 public:

  static Surface *Create(AbstractEventHandler *event_handler, const Margin &margin = Margin());

  static ToplevelShellSurface *Get(const Surface *surface);

  void SetTitle(const char *title) {
    xdg_toplevel_.SetTitle(title);
  }

  void SetAppId(const char *id) {
    xdg_toplevel_.SetAppId(id);
  }

  void Move(const wayland::Seat &seat, uint32_t serial) const {
    xdg_toplevel_.Move(seat, serial);
  }

  void Resize(const wayland::Seat &seat, uint32_t serial, uint32_t edges) const {
    xdg_toplevel_.Resize(seat, serial, edges);
  }

  void SetMaximized() const {
    xdg_toplevel_.SetMaximized();
  }

  void UnsetMaximized() const {
    xdg_toplevel_.UnsetMaximized();
  }

  void SetMinimized() const {
    xdg_toplevel_.SetMinimized();
  }

  DelegateRef<void(int, int, int)> configure() { return xdg_toplevel_.configure(); }

  DelegateRef<void()> close() { return xdg_toplevel_.close(); }

 private:

  ToplevelShellSurface(ShellSurface *shell_surface);

  ~ToplevelShellSurface();

  ShellSurface *shell_surface_;

  wayland::XdgToplevel xdg_toplevel_;

};

}

#endif // SKLAND_GUI_TOPLEVEL_SHELL_SURFACE_HPP_
