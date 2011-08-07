/**
  \file     LH_CursorController.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010,2011 Andy Bridges
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

 **/

#include "LH_CursorController.h"
#include "../json.h"

#include "QDebug"

LH_PLUGIN_CLASS(LH_CursorController)

lh_class *LH_CursorController::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorController",
        "Cursor Controller",
        -1, -1
    };

    return &classInfo;
}

const char *LH_CursorController::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;

    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_NOSAVE | LH_FLAG_READONLY | LH_FLAG_FIRST);
    setup_coordinate_->setHelp("The current cursor location.");

    cursorModes.append((cursorMode){smSelectDeselect, true , "Activate, Move & Select / Deselect"});
    cursorModes.append((cursorMode){smSelect        , true , "Activate, Move & Select Only (one is always selected)"});
    cursorModes.append((cursorMode){smNone          , true , "Activate & Move Only (Highlighted option is selected)"});
    cursorModes.append((cursorMode){smNone          , false, "Move Only (Highlighted option is selected, always activated)"});
    cursorModes.append((cursorMode){smSelectDeselect, false, "Move & Select / Deselect Only (Always activated)"});
    cursorModes.append((cursorMode){smSelect        , false, "Move & Select Only (Always activated)"});

    //cursor_data_ = (cursorData){1,1,false,0,0,false,0,0,false,false,0,0,(bounds){(minmax){0,0},(minmax){0,0}}};

    QStringList modeNames = QStringList();
    foreach(cursorMode cm, cursorModes) modeNames.append(cm.description);
    setup_mode_ = new LH_Qt_QStringList(this,("Cursor Mode"),modeNames,LH_FLAG_AUTORENDER);
    setup_mode_->setHelp("This allows the user to select how the cursor should behave. Each cursor mode is a combination of several options:<br/>"
                         "<br/>"
                         "<ul>"
                         "<li><b>Activate</b></br>"
                         "If a mode includes \"activation\" then in it's default state the cursor is hidden and nothing happens when using the movement or selection keys. Insetad the user must first press the activation key to display the cursor. Whilst active the cursor is visible, can be moved and, if appropriate, items can be selected and deselected. If activation is not included in the mode, then the cursor is assumed to always be active.</li>"
                         "<li><b>Select</b></br>"
                         "For modes that include selecting, the user must first move the cursor over the desired option and then press the select key before anything happens. If a mode does not include selection, then simply moving the cursor changes the selection immediately.</li>"
                         "</li>"
                         "<li><b>Deselect</b></br>"
                         "Some modes have select/deselect behaviour, specified by this option. With this ability the user can \"turn off\" everything; without it there will always be one item selected.</li>"
                         "</li>"
                         "</ul>");
    connect( setup_mode_, SIGNAL(changed()), this, SLOT(changeMode()) );

    setup_boundry_ = new LH_Qt_QString(this,"Boundries","1,1,3,3",0);
    setup_boundry_->setHelp("This box should contain the borders of the \"box\" the cusror can move in.</br></br>The format is [left],[top],[right],[bottom] <br/><br/>e.g.:<br/>\"1,1,1,4\" would denote a box one column wide and four rows high<br/>\"1,1,3,3\" would be a square of three columns and three rows.");
    connect( setup_boundry_, SIGNAL(changed()), this, SLOT(changeBounds()) );
    setup_boundry_loop_ = new LH_Qt_bool(this, "Loop at edges", false, LH_FLAG_BLANKTITLE);
    setup_boundry_loop_->setHelp("By default when at the furthest edge of the box the cursor stops. Ticking this however will change that so that moving off one edge will bring the cursor back in on the opposite side.");

    setup_invalid_coordinates_ = new LH_Qt_QString(this,"Invalid Points\n(x,y;x,y;x,y)","",0);
    setup_invalid_coordinates_->setHelp("If you wish to create a \"void\" in the cusrsor space, this allows you to do so. List all the points here that the cursor is not allowed to land on and it will be prevented from doing so either by jumping over the invlid points to the next valid one or simply by blocking its movement.<br/><br/>The format is [x1],[y1];[x2],[y2];[x3][y3]; etc...<br/>e.g.: for the boundry \"1,1,2,5\" the invalid points \"1,2;1,3;1,4\" would prevent the cursor landing on any of the top three middle cells, creating a \"U\"-like shape.");
    setup_jump_invalid_ = new LH_Qt_bool(this, "Jump over invalid points", true, LH_FLAG_BLANKTITLE);
    setup_jump_invalid_->setHelp("By default when the cursor tries to move onto an invlid point it \"jumps\" the cell in question, and keeps moving in the same direction until reaching one where it is allowed to be. Alternatively, checking this box will prevent this, turning invalid points into \"walls\" that have to be moved around manually.");

    setup_move_up_ = new LH_Qt_InputState(this,"Up","",0);
    setup_move_up_->setHelp("The key used to move the cursor up, usually the up key from the LCD control");
    connect( setup_move_up_, SIGNAL(input(int,int)), this, SLOT(doMoveUp(int,int)) );

    setup_move_down_ = new LH_Qt_InputState(this,"Down","",0);
    setup_move_down_->setHelp("The key used to move the cursor down, usually the down key from the LCD control");
    connect( setup_move_down_, SIGNAL(input(int,int)), this, SLOT(doMoveDown(int,int)) );

    setup_move_left_ = new LH_Qt_InputState(this,"Left","",0);
    setup_move_left_->setHelp("The key used to move the cursor left, usually the left key from the LCD control");
    connect( setup_move_left_, SIGNAL(input(int,int)), this, SLOT(doMoveLeft(int,int)) );

    setup_move_right_ = new LH_Qt_InputState(this,"Right","",0);
    setup_move_right_->setHelp("The key used to move the cursor right, usually the right key from the LCD control");
    connect( setup_move_right_, SIGNAL(input(int,int)), this, SLOT(doMoveRight(int,int)) );

    setup_select_ = new LH_Qt_InputState(this,"Select","",0);
    setup_select_->setHelp("The key used to select the currently highlighted option, usually the Ok key from the LCD control");
    connect( setup_select_, SIGNAL(input(int,int)), this, SLOT(doSelect(int,int)) );

    setup_reselect_ = new LH_Qt_InputState(this,"Reselect","",0);
    setup_reselect_->setHelp("The key used to reselect the previously selected highlighted option. Usually this is unused, but is very useful for menu layouts where it acts as a \"return to last layout\" (or cancel) command");
    connect( setup_reselect_, SIGNAL(input(int,int)), this, SLOT(doReselect(int,int)) );

    setup_activate_ = new LH_Qt_InputState(this,"Enable","",0);
    setup_activate_->setHelp("The key used to activate the cursor control");
    connect( setup_activate_, SIGNAL(input(int,int)), this, SLOT(doActivate(int,int)) );

    setup_persistent_ = new LH_Qt_bool(this,"Persistent Selection",false, LH_FLAG_BLANKTITLE);
    setup_persistent_->setHelp("Ticking this box will cause the system to automatically recall the cursor's last position whether you save the layout or not.<br/><br/>This is mainly of use within menu layouts and allows the menu to open with the last selected option still highlighted.");
    setup_persistent_autoselect_ = new LH_Qt_bool(this,"Auto-reselect",false, LH_FLAG_HIDDEN | LH_FLAG_BLANKTITLE);
    setup_persistent_autoselect_->setHelp("When recovering a persited selection, this option determins if the option is merely highlighed (i.e. the cursor is positioned on it) or if the option is actually selected.");
    setup_persistent_file_ = new LH_Qt_QFileInfo(this, "Persistence File", QFileInfo(), LH_FLAG_HIDDEN);
    setup_persistent_file_->setHelp("The file in which to save the persisted location.<br/><br/>Note that you can use this to make serveral layouts work together as though they were all parts of the same layout by pointing them all to this file, thus causing the cursor position to persist accross them.");

#ifdef ENABLE_VIRTUAL_CURSOR_KEYS
    setup_virtual_keys_ = new LH_Qt_QString(this, "Virtual Keys","",  LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_LAST | LH_FLAG_BLANKTITLE, lh_type_string_htmlhelp);
    setup_virtual_keys_->setHelp("<table>"
                                 "<tr><td>  </td><td><a href='up'>[  Up  ]</a></td><td>  </td></tr>"
                                 "<tr><td><a href='left'>[Left]</a></td><td><a href='sel'>[  OK  ]</a></td><td><a href='right'>[Right]</a></td></tr>"
                                 "<tr><td>  </td><td><a href='down'>[Down]</a></td><td>  </td></tr>"
                                 "<tr><td>  </td><td>  </td><td>  </td></tr>"
                                 "<tr><td><a href='act'>[ Act ]</a></td><td>  </td><td><a href='resel'>[  Re  ]</a></td></tr>"
                                 "</table>");
    connect(setup_virtual_keys_, SIGNAL(change(QString)), this, SLOT(virtualKeyPress(QString)) );
#endif

    setup_json_data_ = new LH_Qt_QString(this, "Cursor Data", "", LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_LAST /*| LH_FLAG_READONLY | LH_FLAG_HIDEVALUE*/);
    setup_json_data_->setLink("Cursors/#1", true);
    setup_json_data_->setLinkFilter("Cursors");
    setup_json_data_->refreshData();

    setup_duplex_postback_ = new LH_Qt_QString(this, "Cursor Postback", "", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_LAST | LH_FLAG_READONLY );
    setup_duplex_postback_->setLink("Cursors/Postback");
    setup_duplex_postback_->setLinkFilter("CursorPostback");
    setup_duplex_postback_->refreshData();

    connect(setup_duplex_postback_,SIGNAL(changed()),this,SLOT(processPostback()));
    connect(setup_persistent_, SIGNAL(changed()), this, SLOT(changePersistent()));
    connect(setup_persistent_file_, SIGNAL(changed()), this, SLOT(loadPersistedSelection()));

    updateLocation(0,0);
    hide();

    return NULL;
}

void LH_CursorController::doMoveUp(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_data_.active )
    {
        updateLocation(0,-1);
    }
}

void LH_CursorController::doMoveDown(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_data_.active )
    {
        updateLocation(0,1);
    }
}

void LH_CursorController::doMoveLeft(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_data_.active )
    {
        updateLocation(-1,0);
    }
}

void LH_CursorController::doMoveRight(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_data_.active )
    {
        updateLocation(1,0);
    }
}

void LH_CursorController::doSelect(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);

    if( cursor_data_.active )
    {
        cursor_data_.sendSelect = false;
        if(cursorModes[setup_mode_->index()].select==smSelectDeselect &&
           cursor_data_.selState &&
           cursor_data_.selX == cursor_data_.x &&
           cursor_data_.selY == cursor_data_.y)
        {
            cursor_data_.selState = false;
        }
        else
        {
            cursor_data_.selX = cursor_data_.x;
            cursor_data_.selY = cursor_data_.y;
            cursor_data_.selState = true;

            if(cursor_data_.lastSelSet)
            {
                cursor_data_.lastSelX2 = cursor_data_.lastSelX;
                cursor_data_.lastSelY2 = cursor_data_.lastSelY;
            } else {
                cursor_data_.lastSelX2 = cursor_data_.selX;
                cursor_data_.lastSelY2 = cursor_data_.selY;
            }

            cursor_data_.lastSelSet = true;
            cursor_data_.lastSelX = cursor_data_.selX;
            cursor_data_.lastSelY = cursor_data_.selY;
        }
        persistSelection();
    }
    setup_json_data_->setValue( cursor_data_.serialize() );
}

void LH_CursorController::doReselect(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);

    if( cursor_data_.active )
    {
        updateLocation(cursor_data_.lastSelX, cursor_data_.lastSelY, true);
        doSelect();
    }
}

void LH_CursorController::doActivate(int flags,int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);

    if( cursorModes[setup_mode_->index()].activate )
    {
        cursor_data_.active = !cursor_data_.active;
        updateLocation(0,0);
    }
}

void LH_CursorController::persistSelection()
{
    if (!setup_persistent_->value()) return;
    if (setup_persistent_file_->value().isFile())
    {
        QString selString = QString("%1,%2,%3,%4,%5").arg(cursor_data_.x).arg(cursor_data_.y).arg(cursor_data_.selState).arg(cursor_data_.selX).arg(cursor_data_.selY);
        QFile persistFile ( setup_persistent_file_->value().absoluteFilePath() );
        persistFile.open(QIODevice::WriteOnly|QFile::Truncate);
        persistFile.write(selString.toUtf8());
        persistFile.close();
    }
}

void LH_CursorController::loadPersistedSelection()
{
    if (!setup_persistent_->value()) return;
    if (setup_persistent_file_->value().isFile())
    {
        QFile persistFile ( setup_persistent_file_->value().absoluteFilePath() );
        persistFile.open(QIODevice::ReadOnly);
        QString selString = QString(persistFile.readAll());
        persistFile.close();

        QStringList selData = selString.split(',');
        if (selData.count()>=5)
        {
            updateLocation(selData[0].toInt(), selData[1].toInt(), true);

            cursor_data_.lastSelSet = true;
            cursor_data_.lastSelX = cursor_data_.x;
            cursor_data_.lastSelY = cursor_data_.y;
            cursor_data_.lastSelX2 = cursor_data_.x;
            cursor_data_.lastSelY2 = cursor_data_.y;

            if(setup_persistent_autoselect_->value())
            {
                cursor_data_.selState = (selData[2] == "1");
                cursor_data_.selX = selData[3].toInt();
                cursor_data_.selY = selData[4].toInt();
            }
        }
    }
}

void LH_CursorController::updateLocation(int xMod, int yMod, bool absolute)
{
    QStringList boundsList = setup_boundry_->value().split(',');
    QStringList badPoints = setup_invalid_coordinates_->value().split(';');

    bool moved = false;

    int newX = cursor_data_.x;
    int newY = cursor_data_.y;

    if (boundsList.length()>=4)
    {
        cursor_data_.range.x = (minmax){boundsList.at(0).toInt(), boundsList.at(2).toInt()};
        cursor_data_.range.y = (minmax){boundsList.at(1).toInt(), boundsList.at(3).toInt()};

        if (absolute)
        {
            if(xMod >= cursor_data_.range.x.min &&
               xMod <= cursor_data_.range.x.max &&
               yMod >= cursor_data_.range.y.min &&
               yMod <= cursor_data_.range.y.max
               )
            {
                newX = xMod;
                newY = yMod;
                QString point = QString::number(newX) + "," + QString::number(newY);
                if(!badPoints.contains(point)) moved = true;
            }
        } else
        if(xMod!=0 || yMod!=0)
        {
            int loops = 0;
            while (loops<=1 && !moved)
            {
                loops++;
                while(!moved &&
                      newX+xMod >= cursor_data_.range.x.min &&
                      newX+xMod <= cursor_data_.range.x.max &&
                      newY+yMod >= cursor_data_.range.y.min &&
                      newY+yMod <= cursor_data_.range.y.max
                      )
                {
                    newX += xMod;
                    newY += yMod;
                    QString point = QString::number(newX) + "," + QString::number(newY);
                    if(!badPoints.contains(point)) moved = true;
                    if(!setup_jump_invalid_->value()) break;


                }
                if(!setup_boundry_loop_->value()) break;
                else if(!moved)
                {
                    if(xMod==1)  newX = cursor_data_.range.x.min - xMod;
                    if(xMod==-1) newX = cursor_data_.range.x.max - xMod;
                    if(yMod==1)  newY = cursor_data_.range.y.min - yMod;
                    if(yMod==-1) newY = cursor_data_.range.y.max - yMod;
                }
            }
        }
    }

    if(moved) {
        cursor_data_.x = newX;
        cursor_data_.y = newY;
        if(cursorModes[setup_mode_->index()].select==smNone) doSelect(0,0);
    }

    persistSelection();

    setup_json_data_->setValue( cursor_data_.serialize() );

    setup_coordinate_->setValue(QString("%1,%2").arg(cursor_data_.x).arg(cursor_data_.y));
}

void LH_CursorController::changeMode()
{
    if(cursorModes[setup_mode_->index()].select!=smSelectDeselect)
    {
        cursor_data_.selX = cursor_data_.x;
        cursor_data_.selY = cursor_data_.y;
        cursor_data_.selState = true;
    }
    cursor_data_.active = !cursorModes[setup_mode_->index()].activate;
    setup_select_->setFlag(LH_FLAG_HIDDEN, cursorModes[setup_mode_->index()].select==smNone);
    setup_reselect_->setFlag(LH_FLAG_HIDDEN, cursorModes[setup_mode_->index()].select!=smSelectDeselect);
    setup_activate_->setFlag(LH_FLAG_HIDDEN, !cursorModes[setup_mode_->index()].activate);
}

void LH_CursorController::changeBounds()
{
    QStringList bounds = setup_boundry_->value().split(',');

    cursor_data_.range.x = (minmax){bounds.at(0).toInt(), bounds.at(2).toInt()};
    cursor_data_.range.y = (minmax){bounds.at(1).toInt(), bounds.at(3).toInt()};

    bool showHorz = false;
    bool showVert = false;
    if(bounds.length()>=4)
    {
        showHorz = bounds.at(0) != bounds.at(2);
        showVert = bounds.at(1) != bounds.at(3);

        setup_move_up_->setFlag(   LH_FLAG_HIDDEN, !showVert);
        setup_move_down_->setFlag( LH_FLAG_HIDDEN, !showVert);
        setup_move_left_->setFlag( LH_FLAG_HIDDEN, !showHorz);
        setup_move_right_->setFlag(LH_FLAG_HIDDEN, !showHorz);
    }
}

void LH_CursorController::changePersistent()
{
    setup_persistent_file_->setFlag(LH_FLAG_HIDDEN, !setup_persistent_->value());
    setup_persistent_autoselect_->setFlag(LH_FLAG_HIDDEN, !setup_persistent_->value());
}

#ifdef ENABLE_VIRTUAL_CURSOR_KEYS
void LH_CursorController::virtualKeyPress(QString s)
{
    if( s == "up" ) doMoveUp();
    else if( s == "down" ) doMoveDown();
    else if( s == "left" ) doMoveLeft();
    else if( s == "right" ) doMoveRight();
    else if( s == "sel" ) doSelect();
    else if( s == "act" ) doActivate();
    else if( s == "resel" ) doReselect();
}
#endif

void LH_CursorController::processPostback()
{
    QString key = setup_json_data_->link();
    if(postback_data.contains(key))
    {
        qDebug() << "CURSOR POSTBACK: " << cursor_data_.deserialize(postback_data.value(key));
        postback_data.remove(key);
        if(cursor_data_.sendSelect) doSelect();
        setup_coordinate_->setValue(QString("%1,%2").arg(cursor_data_.x).arg(cursor_data_.y));
    }
}
