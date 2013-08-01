/*
 * Luca Milani (mail at lucamilani dot eu)
 * 12/07/2013
 */

#include "onoffslider.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

OnOffSlider::OnOffSlider(QWidget *parent) :
    QWidget(parent)
{
    _target       = 75;
    _position     = 75;
    _locked       = false;
    _textPosition = PositionRight;
    _mode         = BiState;
    _currentState = Undefined;
    _appearance   = Horizontal;
}

OnOffSlider::OnOffSlider(const QString &text, QWidget *parent) :
    QWidget(parent), _text(text)
{
    _target       = 75;
    _position     = 75;
    _locked       = false;
    _textPosition = PositionRight;
    _mode         = BiState;
    _currentState = Off;
    _appearance   = Horizontal;
}

OnOffSlider::~OnOffSlider()
{
}

void OnOffSlider::setState(State state)
{
    _currentState = state;

    switch(state)
    {
    case WaitingOn:
    case On:
        _target = 7;
        break;

    case WaitingOff:
    case Off:
        _target = 75;
        break;
    }

    emit stateChanged(state);

    update();
}

void OnOffSlider::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true );

    double currentWidth = 160.;
    switch(_textPosition)
    {
    case PositionTop:
    case PositionBottom:
        // Nothing to do
        break;

    case PositionLeft:
    case PositionRight:
    default:
        currentWidth *= 2;
        break;
    }

    qreal scale = ((qreal)this->width())/currentWidth;
    QMatrix matrix;
    matrix.scale(scale,scale);
    p.setTransform( QTransform(matrix) );

    QFontMetrics fm(QApplication::font());
    switch(_textPosition)
    {
    case PositionTop:
        p.translate( 0, fm.height() );
        break;

    case PositionBottom:
    case PositionRight:
        // Nothing to do
        break;

    case PositionLeft:
        p.translate( fm.width(_text)+5, 0 );
        break;
    }

    QFont font(QApplication::font());
    font.setPointSize(24);

    QPen whitePen;
    whitePen.setWidth(4);
    whitePen.setColor(Qt::white);

    QPen grayPen;
    grayPen.setWidth(1.5);
    grayPen.setColor(Qt::gray);

    QPen blackPen;
    blackPen.setWidth(1);
    blackPen.setColor(Qt::black);

    QLinearGradient lg( QPoint(4,25), QPoint(150,25) );
    if( _mode == BiState ||
        (_mode == FourState && _currentState == Off) )
    {
        lg.setColorAt( 0.00, Qt::red   );
        lg.setColorAt( 0.49, Qt::red   );
    }
    else
    if( _mode == FourState && (_currentState == WaitingOff || _currentState == WaitingOn) )
    {
        lg.setColorAt( 0.00, Qt::yellow   );
        lg.setColorAt( 0.49, Qt::yellow   );
    }

    if( (_mode == BiState && _currentState == WaitingOn) ||
        _currentState == On || _currentState == Off )
    {
        lg.setColorAt( 0.51, Qt::darkGreen );
        lg.setColorAt( 1.00, Qt::darkGreen );
    }
    else
    {
        lg.setColorAt( 0.51, Qt::yellow );
        lg.setColorAt( 1.00, Qt::yellow );
    }

    QLinearGradient dg( QPoint(0,0), QPoint(75,50) );
    dg.setColorAt(0, Qt::gray);
    dg.setColorAt(1, Qt::white);

    p.save();
    p.setPen(whitePen);
    p.drawRoundedRect( QRect(4,4,150,50), 5, 5 );
    p.setPen(grayPen);
    p.drawRoundedRect( QRect(4,4,150,50), 5, 5 );
    p.restore();

    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(lg);
    p.drawRoundedRect( QRect( 6, 6, 146, 46 ), 4, 4 );
    p.restore();

    p.save();
    p.setPen( QColor(255,255,255,200) );
    p.setFont(font);
    p.drawText( QRect( 6, 6,  71, 44 ),  Qt::AlignHCenter | Qt::AlignVCenter, "OFF" );
    p.drawText( QRect( 75, 6, 75, 44 ),  Qt::AlignHCenter | Qt::AlignVCenter, "ON" );
    p.restore();

    // Disegno il tasto
    p.save();
    p.setPen(blackPen);
    p.setBrush(dg);
    if( ((_mode == BiState && _currentState == On) ||
        (_mode == FourState && (_currentState == On || _currentState == WaitingOn))) && _position == _target )
    {
        p.drawRoundedRect( QRect( 7, 7, 75, 44 ), 3, 3 );
    }
    else if( _position == _target )
    {
        p.drawRoundedRect( QRect( 76, 7, 75, 44 ), 3, 3 );
    }
    else
    {
        p.drawRoundedRect( QRect( _position, 7, 75, 44 ), 3, 3 );
        if(_target < _position) _position -= 2;
        else                    _position += 2;
        QTimer::singleShot( 4, this, SLOT(update()) );
    }
    p.restore();

    // Disegno le linee
    p.save();
    p.setPen(Qt::darkGray);
    for( int i = 0; i < 3; i++ )
    {
        p.drawLine( QPoint( _position+75/3+i*(75/3)/2, 12 ), QPoint( _position+75/3+i*(75/3)/2, 46) );
    }
    p.restore();

    p.resetTransform();

    p.save();
    p.setPen(Qt::black);
    p.setFont(QApplication::font());

    QRect textRect;
    switch(_textPosition)
    {
    case PositionTop:
        textRect = QRect( QPoint(0, 0), QSize(160*scale, fm.height()) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignHCenter, _text );
        break;

    case PositionBottom:
        textRect = QRect( QPoint(0, 54*scale), QSize(160*scale, fm.height()) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignHCenter, _text );
        break;

    case PositionLeft:
        textRect = QRect( QPoint(0, 0), QSize(fm.width(_text), 54*scale) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignRight, _text );
        break;

    case PositionRight:
        textRect = QRect( QPoint(160*scale, 0), QSize(fm.width(_text), 54*scale) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignLeft, _text );
        break;
    }
    p.restore();
}

void OnOffSlider::mousePressEvent(QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton )
    {
        nextState();
        emit clicked();
    }
}
