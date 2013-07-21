/*
 * Luca Milani (mail at lucamilani dot eu)
 * (c) 2013 Luca Milani all rights reserved
 */

#ifndef ONOFFSLIDER_H
#define ONOFFSLIDER_H

#include <QWidget>
#include <QCheckBox>

class OnOffSlider : public QCheckBox
{
    Q_OBJECT
    
public:
    explicit OnOffSlider(QWidget *parent = 0);
    OnOffSlider(const QString &text, QWidget *parent = 0);
    ~OnOffSlider();

    enum TextPosition { PositionTop, PositionBottom, PositionLeft, PositionRight };

public:
    void setTextPosition(TextPosition pos) { _textPosition = pos; }
    
private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);

protected: // members
    int  _position;
    int  _target;
    TextPosition _textPosition;
};

#endif // ONOFFSLIDER_H
