#ifndef CUASONGUOIDUNG_H
#define CUASONGUOIDUNG_H

#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <QTime>
#include "ui_CuaSoNguoiDung.h"
#include <QMessageBox>
#include <QMap>
class CuaSoNguoiDung : public QWidget, private Ui::CuaSoNguoiDung {
    Q_OBJECT

    public:
        CuaSoNguoiDung();

    private slots:
        void anNutKetNoi();
        void anNutGuiTin();
        void anEnterGuiTin();
        void nhanDuLieu();
        void ketNoi();
        void ngatKetNoi();
        void loiSocket(QAbstractSocket::SocketError loi);

    private:
        QTcpSocket *socket; // May chu
        quint16 kichThuoc;

};


#endif
