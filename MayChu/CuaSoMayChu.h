#ifndef CUASOMAYCHU_H
#define CUASOMAYCHU_H

#include <QtWidgets>
#include <QtNetwork>
#include <QMap>
#include <QTime>

class CuaSoMayChu : public QWidget {
    Q_OBJECT

    public:
        CuaSoMayChu();
        void guiTinNhanChoMoiNguoi(const QString &tinNhan);
        void guiTinNhanChoNguoiHt(const QString &tinNhan);
        void guiTinNhanChoNguoiBatky(const QString &tinNhan);

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
        QTcpSocket *nguoiDungMoi;
        QTcpSocket *nguoiDungHt;
        QTcpSocket *nguoidungbatky;
        QTcpSocket *chuphong;
        QList<QString> dsonline;
        QList<QString> dsoffline;
        QList<QString> banlist;
        QList<QString> modlist;
        QString content;
        QMap < QString,QString> fillterList;
        QMap <QString,QTcpSocket *> infoTcp;
};



#endif

