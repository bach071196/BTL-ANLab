#ifndef CUASOMAYCHU_H
#define CUASOMAYCHU_H

#include <QtWidgets>
#include <QtNetwork>
#include <QMap>

class CuaSoMayChu : public QWidget {
    Q_OBJECT

    public:
        CuaSoMayChu();
        void guiTinNhanChoMoiNguoi(const QString &tinNhan);

    private slots:
        void ketNoiMoi();
        void nhanDuLieu();
        void ngatKetNoi();

    private:
        QLabel *trangThai;
        QPushButton *thoat;

        QTcpServer *mayChu;
        QList<QTcpSocket *> nguoiDung;
        quint16 kichThuoc;
        QMap <QString,QString> infoNguoiDung;
};



#endif
