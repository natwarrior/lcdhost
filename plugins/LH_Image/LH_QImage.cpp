
#include "LH_QImage.h"
#include <QDebug>

/*
LH_PLUGIN_CLASS(LH_QImage)

lh_class *LH_QImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Static",
        "StaticImage2",
        "Static Image 2",
        48,48


    };
    return &classInfo;
}
*/

struct _logo_blob_s
{
    const unsigned len;
    const unsigned sign;
    const unsigned char data[1218];
};
struct _logo_blob_s const _logo_blob_d =
{
    1218, 0xDEADBEEF,
    {
        137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,48,0,0,0,
        48,8,6,0,0,0,87,2,249,135,0,0,0,6,98,75,71,68,0,255,0,255,0,255,
        160,189,167,147,0,0,0,7,116,73,77,69,7,214,5,8,2,5,5,11,126,38,
        169,0,0,4,100,73,68,65,84,104,129,237,153,95,108,20,69,28,199,
        63,179,179,123,253,119,215,210,63,80,254,20,138,45,109,21,49,
        138,2,15,104,98,48,40,248,160,209,168,52,33,70,44,38,242,160,
        47,26,99,212,7,77,52,70,35,104,36,36,24,12,42,198,196,151,154,
        136,241,79,21,19,80,8,37,34,208,10,84,16,11,85,43,45,168,5,122,
        189,82,142,189,153,241,225,164,129,222,85,118,123,219,86,204,
        125,146,205,101,119,230,247,231,123,51,55,51,191,61,200,146,37,
        203,21,141,24,37,191,115,128,133,192,13,64,41,80,12,196,129,83,
        192,113,96,59,176,3,232,31,165,248,35,34,4,172,116,28,113,24,
        48,128,41,43,145,110,221,44,199,93,56,47,87,223,120,93,142,154,
        62,213,118,67,33,161,0,99,89,156,3,222,4,38,101,18,52,168,17,
        168,177,45,54,43,195,236,59,111,203,215,13,245,17,235,150,5,121,
        76,158,36,83,58,38,18,134,239,127,136,243,113,83,63,235,222,237,
        85,238,121,19,87,154,251,129,166,145,4,14,66,64,141,148,236,157,
        80,36,11,26,55,148,91,139,110,206,243,108,216,125,82,113,215,
        138,110,90,14,198,19,90,115,15,240,185,223,224,153,10,8,217,22,
        45,69,69,178,110,223,150,10,57,99,154,237,219,65,111,84,179,184,
        190,75,183,28,140,247,40,69,13,208,235,199,222,242,29,241,82,
        150,43,195,236,198,183,203,71,148,60,64,81,161,197,7,235,202,
        45,33,68,41,240,188,95,251,76,4,8,199,225,217,165,139,242,181,
        159,105,147,142,171,103,57,60,246,112,161,37,37,143,2,57,126,
        108,51,17,112,173,235,82,219,80,31,25,214,135,235,26,62,250,44,
        198,11,107,78,177,230,173,51,180,253,116,126,88,103,15,61,16,
        65,41,194,192,146,12,114,242,197,42,192,116,183,206,52,166,171,
        58,229,218,183,165,194,76,155,98,187,128,17,130,11,159,230,201,
        85,19,210,246,55,93,213,102,250,84,219,5,222,247,147,68,38,35,
        112,125,105,177,76,164,91,42,149,130,251,30,57,169,78,252,145,
        232,6,230,27,67,46,16,49,134,181,111,108,56,195,87,223,156,77,
        235,112,193,220,28,219,182,197,60,63,73,100,34,160,172,180,36,
        189,249,183,187,6,232,232,116,165,82,172,4,246,0,10,136,1,79,
        216,182,56,180,126,83,212,164,179,187,166,38,132,86,166,6,240,
        188,34,140,108,233,72,82,120,228,168,107,23,213,118,168,161,13,
        209,152,190,48,44,91,135,52,153,68,194,108,217,214,60,80,11,164,
        12,221,236,218,16,218,224,0,51,129,118,47,73,100,34,224,53,160,
        53,26,211,195,181,183,1,233,26,127,233,139,105,121,38,170,153,
        80,120,233,8,94,116,63,145,49,16,176,149,212,111,216,11,3,0,209,
        190,84,1,145,240,224,125,190,87,103,153,8,24,202,98,146,167,206,
        203,177,116,184,134,112,193,224,193,160,192,107,208,160,4,92,
        5,124,237,199,32,63,47,245,20,35,173,193,103,158,119,198,160,
        4,132,1,94,122,186,132,229,247,134,47,219,57,18,182,40,43,73,
        93,126,47,250,61,157,242,26,56,200,41,68,85,165,67,85,165,51,
        98,251,3,135,206,67,178,150,56,224,213,38,211,195,92,96,104,13,
        27,63,140,42,219,102,39,112,194,171,93,160,35,144,9,207,189,210,
        195,222,253,113,9,188,234,199,110,220,5,252,118,60,193,83,47,
        246,152,198,79,99,2,88,143,207,162,102,76,4,244,197,52,29,157,
        9,92,215,16,237,211,252,217,163,56,114,204,101,219,206,1,189,
        253,187,115,24,99,250,73,214,2,107,253,250,30,19,1,21,55,253,
        106,162,125,250,146,117,83,8,180,101,177,75,41,54,3,239,0,167,
        71,226,123,76,4,252,147,252,81,224,113,32,1,116,25,67,187,82,
        12,95,32,120,100,76,4,132,11,44,21,235,215,141,192,151,65,251,
        14,84,192,51,47,247,176,255,199,120,202,243,88,191,78,221,181,
        254,99,20,9,65,20,48,82,210,59,244,34,185,57,61,56,206,57,102,
        185,34,40,4,86,59,182,104,115,28,209,46,4,159,0,115,199,59,41,
        175,148,73,201,207,182,45,212,221,75,10,244,138,101,17,51,117,
        178,237,10,129,2,234,199,59,57,47,188,151,155,35,212,238,47,42,
        6,95,147,156,61,86,101,238,184,53,79,89,22,3,192,148,241,78,240,
        223,176,132,192,77,247,206,231,240,142,25,134,228,42,212,48,42,
        129,3,242,83,108,12,118,93,117,106,45,80,93,105,35,4,134,100,
        161,30,56,65,9,56,45,37,61,205,123,206,165,52,236,110,141,99,
        12,2,248,61,160,88,163,198,106,33,48,27,95,159,104,244,241,228,
        244,105,111,158,97,106,171,157,132,148,116,2,185,227,157,224,
        229,8,73,73,19,96,166,148,75,119,78,93,40,97,89,104,41,249,11,
        152,63,90,65,131,254,147,79,0,203,128,219,129,8,208,10,108,2,
        186,3,142,147,37,75,150,255,11,127,3,251,68,161,48,211,237,5,
        49,0,0,0,0,73,69,78,68,174,66,96,130
    }
};

const char *LH_QImage::userInit()
{
    if( const char *err = LH_QtCFInstance::userInit() ) return err;
    setup_image_file_ = new LH_Qt_QFileInfo( this, ("Image"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_show_placeholder_ = new LH_Qt_bool( this, "Show placholder image when empty", true, LH_FLAG_AUTORENDER | LH_FLAG_BLANKTITLE);
    return 0;
}

bool LH_QImage::loadPlaceholderImage(QImage *img)
{
    return img ? img->loadFromData(_logo_blob_d.data, _logo_blob_d.len) : false;
}

QImage *LH_QImage::render_qimage(int w, int h)
{
    if(QImage *img = initImage(w, h))
    {
        const QFileInfo fi(setup_image_file_ ? setup_image_file_->value() : QFileInfo());
        if(!(fi.isFile() && img->load(fi.absoluteFilePath())))
            if(setup_show_placeholder_ && setup_show_placeholder_->value())
                loadPlaceholderImage(img);
        return img;
    }
    return 0;
}
