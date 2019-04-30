/****************************************************************************
** Meta object code from reading C++ file 'RegisterWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RegisterWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RegisterWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RegisterWindow_t {
    QByteArrayData data[11];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegisterWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegisterWindow_t qt_meta_stringdata_RegisterWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RegisterWindow"
QT_MOC_LITERAL(1, 15, 14), // "shotButtonSlot"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "confirmButtonSlot"
QT_MOC_LITERAL(4, 49, 14), // "quitButtonSlot"
QT_MOC_LITERAL(5, 64, 16), // "upFaceButtonSlot"
QT_MOC_LITERAL(6, 81, 21), // "upBatchFaceButtonSlot"
QT_MOC_LITERAL(7, 103, 8), // "quitSlot"
QT_MOC_LITERAL(8, 112, 10), // "paintEvent"
QT_MOC_LITERAL(9, 123, 12), // "QPaintEvent*"
QT_MOC_LITERAL(10, 136, 1) // "e"

    },
    "RegisterWindow\0shotButtonSlot\0\0"
    "confirmButtonSlot\0quitButtonSlot\0"
    "upFaceButtonSlot\0upBatchFaceButtonSlot\0"
    "quitSlot\0paintEvent\0QPaintEvent*\0e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegisterWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void RegisterWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RegisterWindow *_t = static_cast<RegisterWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->shotButtonSlot(); break;
        case 1: _t->confirmButtonSlot(); break;
        case 2: _t->quitButtonSlot(); break;
        case 3: _t->upFaceButtonSlot(); break;
        case 4: _t->upBatchFaceButtonSlot(); break;
        case 5: _t->quitSlot(); break;
        case 6: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject RegisterWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RegisterWindow.data,
      qt_meta_data_RegisterWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RegisterWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegisterWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterWindow.stringdata0))
        return static_cast<void*>(const_cast< RegisterWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RegisterWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
