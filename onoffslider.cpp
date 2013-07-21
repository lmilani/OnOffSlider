/*
 * Luca Milani (mail at lucamilani dot eu)
 * (c) 2013 Luca Milani all rights reserved
 */

#include "onoffslider.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

OnOffSlider::OnOffSlider(QWidget *parent) :
    QCheckBox(parent)
{
    _target       = 75;
    _position     = 75;
    _textPosition = PositionRight;
}

OnOffSlider::OnOffSlider(const QString &text, QWidget *parent) :
    QCheckBox(text, parent)
{
    _target       = 75;
    _position     = 75;
    _textPosition = PositionRight;
}

OnOffSlider::~OnOffSlider()
{
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
        p.translate( fm.width(this->text())+5, 0 );
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
    lg.setColorAt( 0.00, Qt::red   );
    lg.setColorAt( 0.49, Qt::red   );
    lg.setColorAt( 0.51, Qt::darkGreen   );
    lg.setColorAt( 1.00, Qt::darkGreen );

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

    p.save();
    p.setPen(blackPen);
    p.setBrush(dg);
    if(this->isChecked() && _position == _target )
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
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignHCenter, this->text() );
        break;

    case PositionBottom:
        textRect = QRect( QPoint(0, 54*scale), QSize(160*scale, fm.height()) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignHCenter, this->text() );
        break;

    case PositionLeft:
        textRect = QRect( QPoint(0, 0), QSize(fm.width(this->text()), 54*scale) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignRight, this->text() );
        break;

    case PositionRight:
        textRect = QRect( QPoint(160*scale, 0), QSize(fm.width(this->text()), 54*scale) );
        p.drawText( textRect, Qt::AlignVCenter | Qt::AlignLeft, this->text() );
        break;
    }
    p.restore();
}

void OnOffSlider::mousePressEvent(QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton )
    {
        if( this->isChecked() )
        {
            _target = 75;
        }
        else
        {
            _target = 7;
        }
        this->setChecked( !this->isChecked() );
    }
}
