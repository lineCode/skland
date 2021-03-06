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

#ifndef SKLAND_WAYLAND_SHELL_HPP_
#define SKLAND_WAYLAND_SHELL_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {

class ShellSurface;

class Shell {

  friend class ShellSurface;

  Shell(const Shell &) = delete;
  Shell &operator=(const Shell &) = delete;

 public:

  Shell()
      : wl_shell_(nullptr) {}

  ~Shell() {
    if (wl_shell_) wl_shell_destroy(wl_shell_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_shell_ = static_cast<struct wl_shell *>(registry.Bind(id, &wl_shell_interface, version));
  }

  void Destroy() {
    if (wl_shell_) {
      wl_shell_destroy(wl_shell_);
      wl_shell_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_shell_set_user_data(wl_shell_, user_data);
  }

  void *GetUserData() const {
    return wl_shell_get_user_data(wl_shell_);
  }

  uint32_t GetVersion() const {
    return wl_shell_get_version(wl_shell_);
  }

  bool IsValid() const {
    return nullptr != wl_shell_;
  }

  bool Equal(const void *object) const {
    return wl_shell_ == object;
  }

 private:

  struct wl_shell *wl_shell_;
};

}
}

#endif // SKLAND_WAYLAND_SHELL_HPP_
