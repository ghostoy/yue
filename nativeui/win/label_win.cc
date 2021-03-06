// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "nativeui/label.h"

#include "base/strings/utf_string_conversions.h"
#include "nativeui/gfx/geometry/size_conversions.h"
#include "nativeui/gfx/screen.h"
#include "nativeui/gfx/win/text_win.h"
#include "nativeui/win/view_win.h"

namespace nu {

namespace {

class LabelImpl : public ViewImpl {
 public:
  explicit LabelImpl(Label* delegate)
      : ViewImpl(ControlType::View, delegate) {}

  void SetText(const base::string16& text) {
    text_ = text;
  }

  base::string16 GetText() const {
    return text_;
  }

  // ViewImpl:
  void Draw(PainterWin* painter, const Rect& dirty) override {
    ViewImpl::Draw(painter, dirty);
    TextAttributes attributes(font(), color(), TextAlign::Center,
                              TextAlign::Center);
    painter->DrawTextPixel(text_, Rect(size_allocation().size()), attributes);
  }

 private:
  base::string16 text_;
};

}  // namespace

Label::Label(const std::string& text) {
  TakeOverView(new LabelImpl(this));
  SetText(text);
}

Label::~Label() {
}

void Label::SetText(const std::string& text) {
  LabelImpl* label = static_cast<LabelImpl*>(GetNative());
  base::string16 wtext = base::UTF8ToUTF16(text);
  label->SetText(wtext);

  SetDefaultStyle(ScaleSize(MeasureText(wtext, GetNative()->font()),
                            1.0f / GetScaleFactor()));
  label->Invalidate();
}

std::string Label::GetText() {
  LabelImpl* label = static_cast<LabelImpl*>(GetNative());
  return base::UTF16ToUTF8(label->GetText());
}

}  // namespace nu
