/**
  \file     LH_WeatherBrowserOpener.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#include <QDebug>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include "LH_WeatherBrowserOpener.h"

LH_PLUGIN_CLASS(LH_WeatherBrowserOpener)

lh_class *LH_WeatherBrowserOpener::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Weather",
        "WeatherBrowserOpener",
        "Weather Browser Opener",
        -1, -1,
        
        
    };

    if( classInfo.width == -1 )
    {
        QFont arial10("Arial",10);
        QFontMetrics fm( arial10 );
        classInfo.height = fm.height();
        classInfo.width = classInfo.height * 4;
    }

    return &classInfo;
}

const char *LH_WeatherBrowserOpener::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;
    setup_browser_ = new LH_Qt_InputState(this,("Open in browser"),QString(),LH_FLAG_AUTORENDER);
    setup_browser_->setHelp("Defining a key here will allow you to open the forecast in your browser for more details");
    setup_browser_->setOrder(-4);
    connect( setup_browser_, SIGNAL(input(QString,int,int)), this, SLOT(openBrowser(QString,int,int)) );
    hide();
    return NULL;
}

void LH_WeatherBrowserOpener::openBrowser(QString key,int flags,int value)
{
    qDebug() << "Open Browser";
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( weather_data.url !="" )
        QDesktopServices::openUrl( QUrl::fromUserInput(weather_data.url) );
}


