/****************************************************************************
** Meta object code from reading C++ file 'virtualmachine.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../libs/virtualmachine.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'virtualmachine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_VirtualMachine_t {
    uint offsetsAndSizes[22];
    char stringdata0[15];
    char stringdata1[29];
    char stringdata2[1];
    char stringdata3[34];
    char stringdata4[27];
    char stringdata5[19];
    char stringdata6[22];
    char stringdata7[4];
    char stringdata8[26];
    char stringdata9[24];
    char stringdata10[40];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VirtualMachine_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VirtualMachine_t qt_meta_stringdata_VirtualMachine = {
    {
        QT_MOC_LITERAL(0, 14),  // "VirtualMachine"
        QT_MOC_LITERAL(15, 28),  // "on_NormalRadioButton_clicked"
        QT_MOC_LITERAL(44, 0),  // ""
        QT_MOC_LITERAL(45, 33),  // "on_PassoAPassoRadioButton_cli..."
        QT_MOC_LITERAL(79, 26),  // "addRowCodigoDaMaquinaTable"
        QT_MOC_LITERAL(106, 18),  // "addRowMemoriaTable"
        QT_MOC_LITERAL(125, 21),  // "removeRowMemoriaTable"
        QT_MOC_LITERAL(147, 3),  // "row"
        QT_MOC_LITERAL(151, 25),  // "on_ExecutarButton_clicked"
        QT_MOC_LITERAL(177, 23),  // "on_actionOpen_triggered"
        QT_MOC_LITERAL(201, 39)   // "on_actionSobreAMaquinaVirtual..."
    },
    "VirtualMachine",
    "on_NormalRadioButton_clicked",
    "",
    "on_PassoAPassoRadioButton_clicked",
    "addRowCodigoDaMaquinaTable",
    "addRowMemoriaTable",
    "removeRowMemoriaTable",
    "row",
    "on_ExecutarButton_clicked",
    "on_actionOpen_triggered",
    "on_actionSobreAMaquinaVirtual_triggered"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VirtualMachine[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    4,   64,    2, 0x08,    3 /* Private */,
       5,    2,   73,    2, 0x08,    8 /* Private */,
       6,    1,   78,    2, 0x08,   11 /* Private */,
       8,    0,   81,    2, 0x08,   13 /* Private */,
       9,    0,   82,    2, 0x08,   14 /* Private */,
      10,    0,   83,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject VirtualMachine::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_VirtualMachine.offsetsAndSizes,
    qt_meta_data_VirtualMachine,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_VirtualMachine_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VirtualMachine, std::true_type>,
        // method 'on_NormalRadioButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_PassoAPassoRadioButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addRowCodigoDaMaquinaTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'addRowMemoriaTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'removeRowMemoriaTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_ExecutarButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionOpen_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSobreAMaquinaVirtual_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void VirtualMachine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VirtualMachine *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_NormalRadioButton_clicked(); break;
        case 1: _t->on_PassoAPassoRadioButton_clicked(); break;
        case 2: _t->addRowCodigoDaMaquinaTable((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 3: _t->addRowMemoriaTable((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->removeRowMemoriaTable((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->on_ExecutarButton_clicked(); break;
        case 6: _t->on_actionOpen_triggered(); break;
        case 7: _t->on_actionSobreAMaquinaVirtual_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject *VirtualMachine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualMachine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualMachine.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int VirtualMachine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
