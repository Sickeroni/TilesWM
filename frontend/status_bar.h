#ifndef __STATUS_BAR_H__
#define __STATUS_BAR_H__

#include "child_widget.h"

class StatusBar final : public ChildWidget
{
public:
    StatusBar();
    ~StatusBar();

    virtual void draw(Canvas *canvas) override;

    void setText(const std::string &text);

private:
    std::string _text;
};

#endif
