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

#ifndef SKLAND_GUI_SURFACE_HOLDER_HPP_
#define SKLAND_GUI_SURFACE_HOLDER_HPP_

#include "view-surface.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief The basic class acts as a surface role
 */
class SurfaceHolder {

  SurfaceHolder() = delete;

 public:

  SurfaceHolder(AbstractView *view, const Margin &margin = Margin());

  SurfaceHolder(ViewSurface *surface);

  SurfaceHolder(const SurfaceHolder &other);

  ~SurfaceHolder();

  SurfaceHolder &operator=(const SurfaceHolder &other);

  ViewSurface *view_surface() const { return view_surface_; }

  const wayland::Surface &wl_surface(ViewSurface *view_surface = nullptr) const {
    return nullptr == view_surface ? view_surface_->wl_surface_ : view_surface->wl_surface_;
  }

  SignalRef<> destroyed() const { return view_surface_->destroyed_; }

  void SetParent(ViewSurface *parent);

  /**
   * @brief Move the local surface list and insert above target dst surface
   * @param dst
   */
  void MoveAbove(ViewSurface *dst);

  /**
   * @brief Move the local surface list and insert below target dst surface
   * @param dst
   */
  void MoveBelow(ViewSurface *dst);

  void SetRelativePosition(int x, int y) {
    view_surface_->relative_position_.x = x;
    view_surface_->relative_position_.y = y;
  }

  void PushShellSurface();

  void RemoveShellSurface();

 private:

  void InsertAbove(ViewSurface *sibling);

  void InsertBelow(ViewSurface *sibling);

  ViewSurface *view_surface_;

};

}

#endif // SKLAND_GUI_SURFACE_HOLDER_HPP_
