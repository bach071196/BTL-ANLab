#include "CuaSoMayChu.h"

CuaSoMayChu::CuaSoMayChu () {
    // Tao va sap xep widget
    trangThai = new QLabel();
    thoat = new QPushButton(tr("Thoát"));
    connect(thoat, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *lop = new QVBoxLayout();
    lop->addWidget(trangThai);
    lop->addWidget(thoat);
    setLayout(lop);

    setWindowTitle(tr("Máy chủ Chat room!"));

    // Quan ly may chu
    mayChu = new QTcpServer(this);
    if (!mayChu->listen(QHostAddress::Any, 1080)) { // Khoi dong may chu o cong 1080 và tren tat ca cac dia chi IP cua may
        // Neu may chu chua duoc khoi dong
        trangThai->setText(tr("Máy chủ chưa khởi động được. Lý do :<br />") +mayChu->errorString());
    } else {
        // Neu may chu duoc khoi dong thanh cong
        trangThai->setText(tr("Máy chủ được mở trên cổng <strong>") + QString::number(mayChu->serverPort()) + tr("</strong>.<br />Người dùng có thể nhanh chóng kết nối để tán gẫu."));
        connect(mayChu, SIGNAL(newConnection()), this, SLOT(ketNoiMoi()));
    }

    kichThuoc = 0;
}


void CuaSoMayChu::ketNoiMoi() {
    guiTinNhanChoMoiNguoi (tr("<em>Một người mới  vừa tham gia với chúng ta !</em>"));

    QTcpSocket *nguoiDungMoi = mayChu->nextPendingConnection();
    nguoiDung << nguoiDungMoi;

    connect(nguoiDungMoi, SIGNAL(readyRead()), this, SLOT(nhanDuLieu()));
    connect(nguoiDungMoi, SIGNAL(disconnected()), this, SLOT(ngatKetNoi()));
}

void CuaSoMayChu::nhanDuLieu() {
QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) { // Neu khong xac dinh duoc nguon phat, chung ta se dung xu ly
        return;
    }
    QDataStream in(socket);
    if (kichThuoc == 0) { //Neu chua biet kich thuoc tin nhan thi chung ta se thu tim trong goi du lieu vua toi
         if (socket->bytesAvailable() < (int)sizeof(quint16)) { //Kich thuoc goi tin nho hon kich thuc kieu so nguyen
             return;
        }
        in >> kichThuoc; // Neu nhan duoc kich thuoc tin nhan thi lay ra gia tri do

    }
    // Biet kich thuoc, chung ta se kiem tra xem da nhan duoc toan bo tin nhan chua
    if (socket->bytesAvailable() < kichThuoc) { // Neu chua nhan du tin nhan thi thoat xu ly
        return;
    }
    QString tinNhan;
    QString name;
    QString checkMes;
    QString checkCom;

    in >> tinNhan;
   name = tinNhan;
    checkMes = tinNhan;


    name = name.remove(0,9);
    name = name.remove(name.indexOf(" "),(name.size()-name.indexOf(" ")));


    if (name == "Owner")
    {infoNguoiDung.insert(name,"Admin");}
    else
        {infoNguoiDung.insert(name,"User");}

        if  (tinNhan.indexOf("/")!= -1)
        {checkMes=checkMes.remove(0,tinNhan.indexOf("/")+1);

            checkCom = checkMes;
         checkMes = checkMes.remove(checkMes.indexOf(" "),(checkMes.size()-checkMes.indexOf(" ")));}

//          if ((checkMes == "report")&&(infoNguoiDung.value(name) == "Admin")||(infoNguoiDung.value(name) == "Mod"))



//                   NguoiDungMoi->write( infoNguoiDung.keys()) << endl;
//



//        }

             else
    {guiTinNhanChoMoiNguoi(tinNhan);}


    // Dat lai kich thuoc la 0 de cho tin nhan tiep theo
    kichThuoc = 0;

}

void CuaSoMayChu::ngatKetNoi() {
    guiTinNhanChoMoiNguoi(tr("<em>1 người dùng vừa mới rời đi</em>"));

    // Xac dinh xem ai la nguoi ngat ket noi
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Neu khong tim thay nguoi gui tin hieu thi huy bo xu ly
        return;

    nguoiDung.removeOne(socket);

    socket->deleteLater();
}
void CuaSoMayChu::guiTinNhanChoMoiNguoi(const QString &tinNhan) {
    // Chuan bi tin nhan gui di
    QByteArray goiTinNhan;
    QDataStream out(&goiTinNhan, QIODevice::WriteOnly);

    out << (quint16) 0; // Viet gia tri 0 o dau goi tin de dat truoc cho de viet kich thuoc tin nhan
    out << tinNhan; // Viet noi dung tin nhan vao goi tin
    out.device()->seek(0); // Quay ve dau goi tin
    out << (quint16) (goiTinNhan.size() - sizeof(quint16)); // Thay 0 bang gia tri kich thuoc that cua tin nhan

    // Gui tin cho tat ca nguoi dung ket noi
    for (int i = 0; i < nguoiDung.size(); i++) {
        nguoiDung[i]->write(goiTinNhan);
    }

}

