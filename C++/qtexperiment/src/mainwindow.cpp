#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QWindow>
#include <QKeyEvent>
#include <QPen>


//void mainwindow::refresh_selected_hexgrid(){ refresh_hexgrid_cylinder(); }
//void mainwindow::refresh_selected_hexgrid(){ refresh_hexgrid(); }
void mainwindow::refresh_selected_hexgrid(){ refresh_hexgrid_xhatchwaves(); }


void mainwindow::a_slider_changed(int value) { a_value = value; refresh_selected_hexgrid(); }
void mainwindow::b_slider_changed(int value) { b_value = value; refresh_selected_hexgrid(); }


mainwindow::mainwindow( QWidget *parent)
  : QMainWindow(parent), ui(new Ui::mainwindow)
{
    ui->setupUi(this);

  //void QAbstractSlider::valueChanged(int value)

  //scene = new QGraphicsScene( 0, 0, 1560, 860, this );
    scene = new QGraphicsScene( 0, 0, 2048/2, 1080/2, this );
    ui->graphicsView->setScene( scene ); //QGraphicsView

    QPixmap pxmp;
    pixmap_item = scene->addPixmap( pxmp );

    // if the spinbox changes, change the spinbox
    QObject::connect( ui->a_hSlider, &QAbstractSlider::valueChanged, ui->a_spinBox, &QSpinBox::setValue );
    QObject::connect( ui->b_hSlider, &QAbstractSlider::valueChanged, ui->b_spinBox, &QSpinBox::setValue );

    // if the spinbox changes, change the slider
    // is qt smart enough to stop circular recursion?
    //QObject::connect( ui->a_spinBox, &QSpinBox::valueChanged, ui->a_hSlider, &QAbstractSlider::setValue );
    //QObject::connect( ui->b_spinBox, &QSpinBox::valueChanged, ui->b_hSlider, &QAbstractSlider::setValue );


    ui->a_hSlider->setValue( a_value );
    ui->b_hSlider->setValue( b_value );

    QObject::connect( ui->a_hSlider, &QAbstractSlider::valueChanged, this, &mainwindow::a_slider_changed );
    QObject::connect( ui->b_hSlider, &QAbstractSlider::valueChanged, this, &mainwindow::b_slider_changed );

    //refresh_selected_hexgrid();
}

mainwindow::~mainwindow()
{
    delete ui;
    printf( "~mainwindow() deleted ui\n" );
}

void mainwindow::timer_event()
{
    ticks++;
    refresh_selected_hexgrid();
}

bool mainwindow::toggle_timer()
{
    if( tmr_active )
        tmr.stop();
    else
    {
        QObject::connect( &tmr, &QTimer::timeout, this, &mainwindow::timer_event );
        //tmr.start( tmr_interval_msec );
        tmr.start( 1 );
    }
    tmr_active = !  tmr_active;
    return tmr_active;
}


void mainwindow::keyPressEvent( QKeyEvent * e )
{
    switch( e->key() )
    {
    case Qt::Key_Escape:
    case Qt::Key_Q:
        qApp->quit();
        break;
    case Qt::Key_T:
        toggle_timer();
        printf( "ticks: %d\n", ticks );
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        log_tick_stepsize += .01;
#if 0
        tmr_interval_msec++;
        printf( "tmr_interval_msec: %d\n", tmr_interval_msec );
        tmr.setInterval( tmr_interval_msec );
#endif
        break;
    case Qt::Key_Minus:
        log_tick_stepsize -= .01;
#if 0
        if( tmr_interval_msec > 1 )
        {
            tmr_interval_msec--;
            printf( "tmr_interval_msec: %d\n", tmr_interval_msec );
            tmr.setInterval( tmr_interval_msec );
        }
#endif
        break;
    case Qt::Key_F: f_bool = ! f_bool; break;
    case Qt::Key_R: r_bool = ! r_bool; break;
    case Qt::Key_L: l_bool = ! l_bool; break;
    case Qt::Key_P: p_bool = ! p_bool; break;
    case Qt::Key_N: n_bool = ! n_bool; break;
    case Qt::Key_B:
        b_cmd = true;
        break;
    case Qt::Key_F11:
        printf( "F11 pressed\n" );
        setVisible(true);
        if( is_fullscreenmode )
        {
            showMaximized();
            //showFullScreen();
            ui->graphicsView->showFullScreen();
        }
        else
        {
            ui->graphicsView->showNormal();
            showNormal();
        }

        is_fullscreenmode = ! is_fullscreenmode;
        break;
    case Qt::Key_S:
        ticks += 100;
        printf( "ticks: %d\n", ticks );
        break;
    }
}


