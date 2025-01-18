#pragma once

#include <cstdarg>
#include <functional>
#include <list>
#include <memory>
#include <string>

#include "imgui.h"

namespace SImGui {
struct Element {
  std::string name;
  std::list<std::shared_ptr<Element>> childs;

  Element() = default;

  Element(std::string name) : name(name) {}
  Element(std::list<std::shared_ptr<Element>> childs) : childs(childs) {}

  Element(std::string name, std::list<std::shared_ptr<Element>> childs)
      : name(std::move(name)), childs(std::move(childs)) {}

  virtual ~Element() = default;

  virtual void render() {
    for (auto &child : childs) {
      child->render();
    }
  }
};

struct Widget : public Element {
  Widget(std::string id) : Element(id) {}
  std::string get_id() { return this->name; }
};

class CustomWidget : public Widget {
  const std::function<void()> action;

public:
  CustomWidget(const std::function<void()> &action)
      : Widget("##"), action(action) {}

  void render() override { action(); }
};

class Text : public Widget {
  const char *fmt;
  char buffer[1024];

public:
  Text(const char *fmt, ...) : Widget("##"), fmt(fmt) {
    va_list args;
    va_start(args, fmt);

    vsnprintf(this->buffer, sizeof(this->buffer), this->fmt, args);

    va_end(args);
  }

  void render() override { ImGui::Text("%s", buffer); }
};

class Button : public Widget {
  const std::function<void()> action;
  ImVec2 size;

public:
  Button(std::string id, const std::function<void()> &action,
         ImVec2 size = ImVec2(0, 0))
      : Widget(id), action(action), size(size) {}

  void render() override {
    if (ImGui::Button(this->name.c_str(), this->size)) {
      action();
    }
  }
};

class Checkbox : public Widget {
  bool *v;

public:
  Checkbox(std::string id, bool *v) : Widget(id), v(v) {}

  void render() override { ImGui::Checkbox(this->name.c_str(), this->v); }
};

class SliderInt : public Widget {
  int *v;
  int v_min;
  int v_max;
  const char *format;
  ImGuiSliderFlags flags;

public:
  SliderInt(std::string id, int *v, int v_min, int v_max,
            const char *format = "%d", ImGuiSliderFlags flags = 0)
      : Widget(id), v(v), v_min(v_min), v_max(v_max), format(format),
        flags(flags) {}

  void render() override {
    ImGui::SliderInt(this->name.c_str(), this->v, this->v_min, this->v_max,
                     this->format, this->flags);
  }
};

class SliderFloat : public Widget {
  float *v;
  float v_min;
  float v_max;
  const char *format;
  ImGuiSliderFlags flags;

public:
  SliderFloat(std::string id, float *v, float v_min, float v_max,
              const char *format = "%.3f", ImGuiSliderFlags flags = 0)
      : Widget(id), v(v), v_min(v_min), v_max(v_max), format(format),
        flags(flags) {}

  void render() override {
    ImGui::SliderFloat(this->name.c_str(), this->v, this->v_min, this->v_max,
                       this->format, this->flags);
  }
};

class ColorEdit4 : public Widget {
  float *col;
  ImGuiColorEditFlags flags;

public:
  ColorEdit4(std::string id, float *col, ImGuiColorEditFlags flags = 0)
      : Widget(id), col(col), flags(flags) {}

  void render() override {
    ImGui::ColorEdit4(this->name.c_str(), this->col, this->flags);
  }
};

template <typename T, typename... Args>
inline std::shared_ptr<T> make_element(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename... Args>
inline std::shared_ptr<Element> make_element(const std::string &name,
                                             Args &&...args) {
  return std::make_shared<Element>(
      name, std::list<std::shared_ptr<Element>>{std::forward<Args>(args)...});
}

inline std::shared_ptr<Element> find_element(std::shared_ptr<Element> root,
                                             const std::string name) {
  if (!root) {
    return nullptr;
  }

  auto element = std::ranges::find_if(
      root->childs.begin(), root->childs.end(),
      [&](const std::shared_ptr<Element> &e) { return e->name == name; });

  if (element != root->childs.end()) {
    return *element;
  }

  return nullptr;
}

template <typename... Args>
inline std::shared_ptr<Element> find_element(std::shared_ptr<Element> root,
                                             Args &&...args) {
  auto args_v = std::vector<std::string>{std::forward<Args>(args)...};

  std::shared_ptr<Element> last_element = root;
  for (const auto &arg : args_v) {
    auto element = find_element(last_element, arg);

    if (element) {
      last_element = element;
    } else {
      break;
    }
  }

  return last_element == root ? nullptr : last_element;
}
} // namespace SImGui