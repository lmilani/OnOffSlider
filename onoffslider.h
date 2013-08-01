/*
 * Luca Milani (mail at lucamilani dot eu)
 * 12/07/2013
 */

#ifndef ONOFFSLIDER_H
#define ONOFFSLIDER_H

#include <QWidget>
#include <QCheckBox>

class OnOffSlider : public QWidget
{
    Q_OBJECT
    Q_ENUMS(TextPosition)
    Q_ENUMS(Mode)
    
public:
    explicit OnOffSlider(QWidget *parent = 0);
    OnOffSlider(const QString &text, QWidget *parent = 0);
    ~OnOffSlider();

    //! Enumeration for text position
    enum TextPosition { PositionTop, PositionBottom, PositionLeft, PositionRight };

    //! Enumeration for the widget working mode
    enum Mode         { BiState, TriState, FourState };

    //! States enumeration for the 'FourState' working mode
    enum State        { Undefined, On, Off, WaitingOn, WaitingOff };

    //! Aspect appearance
    enum Appearance   { Vertical, Horizontal };

public:
    void setTextPosition(TextPosition pos ) { _textPosition = pos;  }
    void setMode        (Mode         mode)
    {
        _mode = mode;
        _currentState = Off;
    }
    void setText        (QString text)    { _text         = text; }
    void setAppearance  (Appearance   app){ _appearance   = app;  }
    void setState       (State      state);

    State state()       const { return _currentState; }
    State checkState()  const { return _currentState; }
    Mode  mode()        const { return _mode; }
    bool  isChecked()   const { return _currentState == On; }
    bool  isTristate()  const { return _mode == TriState; }
    bool  isFourState() const { return _mode == FourState; }

signals:
    void clicked();
    void stateChanged(State);
    
private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void nextState()
    {
        switch(_mode)
        {
        default:
        case BiState:
            if(_currentState == Off)        setState(On);
            else
            if(_currentState == On)         setState(Off);
            break;

        case TriState:
            if(_currentState == Off)        setState(WaitingOn);
            else
            if(_currentState == WaitingOn)  setState(On);
            else
            if(_currentState == On)         setState(Off);
            break;

        case FourState:
            if(_currentState == Off)        setState(WaitingOn);
            else
            if(_currentState == WaitingOn)  setState(On);
            else
            if(_currentState == On)         setState(WaitingOff);
            else
            if(_currentState == WaitingOff) setState(Off);
            break;
        }
    }

protected: // members
    int          _position;
    int          _target;
    bool         _locked;
    QString      _text;
    TextPosition _textPosition;
    Mode         _mode;
    State        _currentState;
    Appearance   _appearance;
};

#endif // ONOFFSLIDER_H
