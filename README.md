# SImGui

Simple implementation of ImGui menu display in C++

## Example
```cpp
int score = 0;
bool auto_update = false;

const auto update_score = [&] {
    auto score_text = SImGui::find_element(elements, "Content", "ScoreText");

    score_text->childs.clear();
    score_text->childs.emplace_back(
        SImGui::make_element<SImGui::Text>("Score: %d", score));
};

elements->childs.emplace_back(SImGui::make_element(
    "Content", SImGui::make_element<SImGui::Text>("Example of using SImGui"),
    SImGui::make_element<SImGui::Button>("Click!", [&] { score++; }),
    SImGui::make_element(
        "ScoreText", SImGui::make_element<SImGui::Text>("Score: %d", score)),
    SImGui::make_element<SImGui::Checkbox>("Auto update", &auto_update),
    SImGui::make_element<SImGui::CustomWidget>([] { ImGui::SameLine(); }),
    SImGui::make_element<SImGui::Button>("Update score",
                                        [&] { update_score(); })));

if (auto_update) {
    update_score();
}

ImGui::Begin("SImGui");

elements->render();

ImGui::End();
```

### Thanks to @Sanceilaks for helping with the implementation