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

#include <skland/gui/raster-surface.hpp>
#include <skland/gui/buffer.hpp>

#include <skland/graphic/canvas.hpp>

namespace skland {

RasterSurface::RasterSurface(const Margin &margin)
    : AbstractSurface(margin) {}

RasterSurface::~RasterSurface() {
}

void RasterSurface::OnSetup() {
  // TODO: setup when this surface is set in a view
}

void RasterSurface::OnAttach(const Buffer *buffer) {
  if (nullptr == buffer) {
    canvas_.reset(nullptr);
  } else {
    Canvas *canvas = new Canvas((unsigned char *) buffer->pixel(),
                                buffer->size().width,
                                buffer->size().height);
    canvas->SetOrigin((float) margin().left, (float) margin().top);
    canvas_.reset(canvas);
  }
}

}
