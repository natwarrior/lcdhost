/**
  \file     LH_Graph.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh.

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

#ifndef LH_GRAPH_H
#define LH_GRAPH_H

#include "LH_QtInstance.h"
#include "LH_Qt_QColor.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QSlider.h"
#include "LH_Qt_bool.h"
#include "LH_Qt_int.h"
#include "LH_Qt_float.h"
#include "LH_Qt_QTextEdit.h"
#include "LH_Qt_QFont.h"
#include "LH_Qt_QFileInfo.h"
#include "QList"
#include "QHash"

#include "DataCollection.h"

enum GraphDataMode
{
    /* Internally Managed Data means the graph uses it's own internal data store.   *
     * Data points are added to the store (usually via a decendant class) using the *
     * "addValue" routine.                                                          */
    gdmInternallyManaged,

    /* Externally Managed Data means the graph makes no use of it's internal store. *
     * Instead, some 3rd party (possibly a decendant, but not necessarily) is going *
     * to maintain it's own DataLineCollection object which the graph will make use *
     * of.                                                                          */
    gdmExternallyManaged,

    /* "Hybrid" management exists where a 3rd party will be maintaining it's own    *
     * record of data values (in a DataLineCollection object), which will be copied *
     * into the graph object when the data is initialised. However, once this has   *
     * been done, the graph will maintain it's own collection and changes to the    *
     * 3rd party collection will be ignored.                                         */
    gdmHybrid
};

class LH_Graph : public LH_QtInstance
{
    Q_OBJECT

    const static bool isDebug = false;

    qreal dataMaxY_;
    qreal dataMinY_;
    qreal dataDeltaY_;

    qreal graphMaxY_;
    qreal graphMinY_;

    qreal divisorY_;

    QString unitText_;

    QSize img_size_;
    QImage bgImg_;
    QHash<int,QImage> fgImgs_;
    void reload_images();
    void findDataBounds(DataLineCollection* lineData);

    DataLineCollection lines_;
    QVector<int> cacheCount_;
    QVector<qreal> cacheVal_;

    bool userDefinableLimits_;

    qreal max(qreal);
    qreal min(qreal);

    bool hasDeadValue_;
    qreal deadValue_;

    GraphDataMode dataMode_;
    DataLineCollection* externalSource_;
    DataLineCollection* lineData_;
protected:
    bool graph_empty_;

    LH_Qt_QStringList *setup_fg_type_;
    LH_Qt_QStringList *setup_bg_type_;
    LH_Qt_QStringList *setup_orientation_;
    LH_Qt_QStringList *setup_line_selection_;
    LH_Qt_QColor *setup_pencolor_;
    LH_Qt_QColor *setup_fillcolor1_;
    LH_Qt_QColor *setup_fillcolor2_;
    LH_Qt_QTextEdit *setup_line_configs_;
    LH_Qt_QColor *setup_bgcolor_;
    LH_Qt_int *setup_max_samples_;
    LH_Qt_int *setup_sample_rate_;
    LH_Qt_QString *setup_description_;

    LH_Qt_bool *setup_max_grow_;
    LH_Qt_float *setup_max_;
    LH_Qt_float *setup_min_;

    LH_Qt_bool *setup_auto_scale_y_max_;
    LH_Qt_bool *setup_auto_scale_y_min_;
    LH_Qt_bool *setup_show_y_max_;
    LH_Qt_bool *setup_show_y_min_;
    LH_Qt_bool *setup_hide_when_empty_;
    LH_Qt_bool *setup_show_real_limits_;
    LH_Qt_bool *setup_y_labels_right_;
    LH_Qt_QFont *setup_label_font_;
    LH_Qt_QColor *setup_label_color_;
    LH_Qt_QColor *setup_label_shadow_;

    LH_Qt_QFileInfo *setup_fg_image_;
    LH_Qt_QFileInfo *setup_bg_image_;
    LH_Qt_int *setup_fg_alpha_;

    void __ctor( float defaultMin, float defaultMax, GraphDataMode dataMode, DataLineCollection* externalSource );

    void setExternalSource(DataLineCollection* externalSource) {
        if(externalSource_!=externalSource)
            externalSource_=externalSource;
    }
public:
    LH_Graph( float defaultMin = 0, float defaultMax = 0) : lines_(30)
    { __ctor(defaultMin, defaultMax, gdmInternallyManaged, NULL); }

    LH_Graph( GraphDataMode dataMode, DataLineCollection* externalSource) : lines_(30)
    { __ctor(0, 0, dataMode, externalSource); }

    LH_Graph( float defaultMin, float defaultMax, GraphDataMode dataMode, DataLineCollection* externalSource ) : lines_(30)
    { __ctor(defaultMin, defaultMax, dataMode, externalSource); }

    QImage *render_qimage( int w, int h );

    qreal max();
    qreal min();
    bool canGrow();
    bool canGrow(bool);

    bool setMin( qreal r ); // return true if rendering needed
    bool setMax( qreal r, bool b = false ); // return true if rendering needed

    void addLine(QString name);
    int lineCount();
    void clearLines();
    void setLines(QStringList names) {clearLines(); for(int i=0; i<names.length(); i++) addLine(names[i]);}


    void setYUnit( QString str, qreal divisor = 1);

    void drawSingle( int lineID = 0, DataLineCollection* dlc = NULL );
    void drawAll( DataLineCollection* dlc = NULL ) {
        for( int i=0; i<lineCount(); i++ )
        {
#ifdef LH_MONITORING_LIBRARY
            if( dlc->at(i).aggregate )
                continue;
            if( dlc->at(i).group )
                continue;
#endif
            drawSingle( i, dlc );
        }
    }

    void addValue(float value, int lineID = 0);
    void addValues(QVector<float> values ) { for( int i=0; i<values.size(); ++i ) addValue( values.at(i), i ); }

    void updateDescText();
    void loadColors(int lineID, QColor& penColor, QColor& fillColor1, QColor& fillColor2, QString& fgImgPath, int& fgImgAlpha);
    QString buildColorConfig();

    void findDataBounds();
    void addText(QPainter& painter, QRect rect, int flags, QString text);
    void addText(QPainter& painter, QRect rect, int flags, QString text, int Xmod, int Ymod);
    QString getLabelText(qreal val);

    void clear(float newMin=0, float newMax=1, bool newGrow = true);

    bool userDefinableLimits();
    bool setUserDefinableLimits(bool v);

    GraphDataMode dataMode() { return dataMode_; }

    void setDeadValue(qreal v){
        hasDeadValue_ = true;
        deadValue_ = v;
    }

public slots:
    void changeMaxSamples();
    void changeSampleRate();
    void changeType();
    void changeSelectedLine();
    void updateSelectedLine();
    void updateLabelSelection();
    void updateLimitControls();
    void updateFGImage();
    void updateBGImage();
};

#endif // LH_GRAPH_H
