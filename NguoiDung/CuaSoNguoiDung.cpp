#include "CuaSoNguoiDung.h"


CuaSoNguoiDung::CuaSoNguoiDung() {
    setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(nhanDuLieu()));
    connect(socket, SIGNAL(connected()), this, SLOT(ketNoi()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(ngatKetNoi()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(loiSocket(QAbstractSocket::SocketError)));

    connect(nutKetNoi, SIGNAL(clicked()), this, SLOT(anNutKetNoi()));
    connect(nutGuiTin, SIGNAL(clicked()), this, SLOT(anNutGuiTin()));
    connect(khungSoanThao, SIGNAL(returnPressed()), this, SLOT(anEnterGuiTin()));

    kichThuoc = 0;
}

// Thiet lap ket noi den may chu
void CuaSoNguoiDung::anNutKetNoi() {
    // Thong bao la ket noi dang duoc thuc hien
    cuocHoiThoai->append(tr("<em>Đang kết nối...</em>"));
    nutKetNoi->setEnabled(false);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Hãy lựa chọn", "Bạn đang muốn làm gì?" , QMessageBox::Yes | QMessageBox::No | QMessageBox::Ignore);
    //QAbstractButton *myYesButton = msgBox.addButton(trUtf8("Sim"), QMessageBox::YesRole);
    //Private:
    //const BUT_YES As String = "Đồng ý";
    if(reply == QMessageBox::Ignore) {
            QApplication::quit();}
                    else
                    if(reply == QMessageBox::No){
    socket->abort();
    socket->connectToHost(ipMayChu->text(), congMayChu->value());

                    }
                    else {
                        bool ok = false;
                            QString Passwork = QInputDialog::getText(this, "Passwork"," Hãy nhập Passwork ?", QLineEdit::Normal, QString(), &ok);
                            if (ok && Passwork=="123456"){
                                QMessageBox::information(this,"Passwork", "Xin chào Owner !");
                                socket->abort();
                                socket->connectToHost(ipMayChu->text(), congMayChu->value());

                            }else{
                                QMessageBox::critical(this, "Passwork", "Sai mật khẩu, xin thử lại.");
                              QApplication::quit();

                            }
                    }
}

// Gui tin den may chu
void CuaSoNguoiDung::anNutGuiTin() {
    QByteArray goiTin;
    QDataStream out(&goiTin, QIODevice::WriteOnly);
    QTime ct = QTime::currentTime();
    QString mesContent = khungSoanThao ->text();;
    QString name = nickname->text();
//    QString temp ;
//    if (mesContent.indexOf("/") == 0){

//        qDebug()<<"mesContent"<<mesContent;

//        QString command = mesContent;

//         command = command.remove(mesContent.indexOf(" "), mesContent.size() - mesContent.indexOf(" "));



//        temp  = mesContent.remove(0 ,command.size() + 1);



//       switch (temp) {
//       case "/leave":
//            if (name=="Owner")
//            {
//                QMessageBox::StandardButton rep = QMessageBox::question(this, "Hãy lựa chọn", "Bạn muốn thoát khỏi phòng" , QMessageBox::Yes | QMessageBox::No );}
//            if ( rep == QMessageBox::Yes)
//            { ngatKetNoi();}

//            else {
//            ngatKetNoi();
//            }
//            break;
//        case "/nickname":
//            cuocHoiThoai->append("Nickname của bạn là :" + nickname->text() );
//            break;
//        //case 'setnickname':


//        default:
//            cuocHoiThoai->append("Lệnh không rõ, vui lòng nhập lại");



//            break;
//        }

//    }
//    else
    // Chuan bi goi tin de gui di
    {
        QString tinGuiDi = ct.toString()+" "+ nickname->text() + " : " + khungSoanThao ->text();

    out << (quint16) 0;
    out << tinGuiDi;
    out.device()->seek(0);
    out << (quint16) (goiTin.size() - sizeof(quint16));

    socket->write(goiTin); // Gui goi tin

    khungSoanThao ->clear(); // Xoa tin vua gui khoi khung soan thao
    khungSoanThao ->setFocus();
    }
}

void CuaSoNguoiDung::anEnterGuiTin() {
    anNutGuiTin();
}

void CuaSoNguoiDung::nhanDuLieu() {

    QDataStream in(socket);
    if (kichThuoc == 0) {
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
    in >> tinNhan;

    cuocHoiThoai->append(tinNhan);

    // Dat lai kich thuoc la 0 de cho tin nhan tiep theo
    kichThuoc = 0;

}

// Slot kich hoat khi ket noi thanh cong
void CuaSoNguoiDung::ketNoi() {
    cuocHoiThoai->append(tr("<em>Kết nối thành công !</em>"));
    nutKetNoi->setEnabled(true);
}

// Slot kich hoat khi thoat ket noi
void CuaSoNguoiDung::ngatKetNoi() {
    cuocHoiThoai->append(tr("<em>Tạm biệt, hẹn gặp lại sau !</em>"));
}

// Slot kich hoat khi co loi socket
void CuaSoNguoiDung::loiSocket(QAbstractSocket::SocketError loi) {
    switch(loi) { // Hien thi thong bao khac nhau tuy theo loi gap phai

        case QAbstractSocket::HostNotFoundError:
            cuocHoiThoai->append(tr("<em>LỖI : Không thể kết nối tới máy chủ ! Vui lòng kiểm tra lại địa chỉ IP và cổng truy cập.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            cuocHoiThoai->append(tr("<em>LỖI : Máy chủ từ chối truy cập ! Vui lòng kiểm tra chắc chắn là máy chủ đã được khởi động. Lưu ý đồng thời lỗi địa chỉ IP và cổng truy cập.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            cuocHoiThoai->append(tr("<em>LỖI : Máy chủ đã ngắt kết nối !</em>"));
            break;
        default:
            cuocHoiThoai->append(tr("<em>LỖI : ") + socket->errorString() + tr("</em>"));
    }

    nutKetNoi->setEnabled(true);
}
