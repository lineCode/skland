# Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# core module
add_subdirectory(core-margin)
add_subdirectory(core-rect)
add_subdirectory(core-color)
add_subdirectory(core-object)
add_subdirectory(core-vectors)

if (LINUX)
    add_subdirectory(core-posix-timer)
endif ()

if (LINUX)

    # graphic
    add_subdirectory(graphic-font)
    add_subdirectory(graphic-font-style)
    add_subdirectory(graphic-path)
    add_subdirectory(graphic-pattern)
    add_subdirectory(graphic-paint)
    add_subdirectory(graphic-canvas)

    # wayland wrapper
    add_subdirectory(wayland-client-display)
    add_subdirectory(wayland-client-registry)
    add_subdirectory(wayland-client-output)
    add_subdirectory(wayland-client-seat)

    # gui
    add_subdirectory(gui-idle-task)
    add_subdirectory(gui-display)
    add_subdirectory(gui-output)
    add_subdirectory(gui-application)
    add_subdirectory(gui-abstract-widget)
    add_subdirectory(gui-abstract-window)
    add_subdirectory(gui-window)
    add_subdirectory(gui-timer)
    # add_subdirectory(gui-main-window)
    add_subdirectory(gui-slider)
    add_subdirectory(gui-egl-widget)
    add_subdirectory(gui-linear-layout)

endif ()