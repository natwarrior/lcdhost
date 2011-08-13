#ifndef LH_QT_ARRAY_STRING_UI_H
#define LH_QT_ARRAY_STRING_UI_H

#include "LH_Qt_array_string.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QFileInfo.h"
#include <QDebug>

class LH_Qt_array_string_ui : public LH_Qt_array_string
{
    Q_OBJECT

    int uiIndex_;
    LH_QtSetupItem *ui_;
    bool updatingUI_;
    lh_setup_type ui_type_;

    void init(lh_setup_type ui_type, int uiFlags );

public:
    LH_Qt_array_string_ui( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0, lh_setup_type ui_type = lh_type_string)
        : LH_Qt_array_string( parent, ident, size, flags | LH_FLAG_HIDDEN )
    {
        init( ui_type, LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | flags );
    }


    void setEditIndex(int index);

    void setFlag(int f, bool state);

    virtual void setHelp(const QString& s);

    void setTitle(const char *s = 0);

    void setTitle( const QString &s );

public slots:
    void arrayValuesChanged();
    void uiValueChanged();

};

#endif // LH_QT_ARRAY_STRING_UI_H