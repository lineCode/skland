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

#ifndef SKLAND_GUI_EGL_SURFACE_HPP_
#define SKLAND_GUI_EGL_SURFACE_HPP_

#include "../egl/surface.hpp"

namespace skland {

class Surface;

class EGLSurface {

  friend class Surface;

  EGLSurface() = delete;
  EGLSurface(const EGLSurface &) = delete;
  EGLSurface &operator=(const EGLSurface &) = delete;

 public:

  static EGLSurface *Get(Surface *surface);

  virtual ~EGLSurface();

  bool MakeCurrent();

  bool SwapBuffers();

  bool SwapBuffersWithDamage(int x, int y, int width, int height);

  bool SwapInterval(EGLint interval = 0);

  void Resize(int width, int height, int dx = 0, int dy = 0) {
    egl_surface_.Resize(width, height, dx, dy);
  }

  Surface *surface() const { return surface_; }

 private:

  EGLSurface(Surface *surface);

  Surface *surface_;

  egl::Surface egl_surface_;

};

}

#endif // SKLAND_GUI_EGL_SURFACE_HPP_
