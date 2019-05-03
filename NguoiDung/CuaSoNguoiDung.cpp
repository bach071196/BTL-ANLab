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

    if(reply == QMessageBox::Ignore) {
            QApplication::quit();}
                    else
                    if(reply == QMessageBox::No){
                        bool ok = false;
                            QString name = QInputDialog::getText(this, "Tên đăng nhập","Nhập nickname bạn muốn sử dụng ", QLineEdit::Normal, QString(), &ok);
                            if (ok){
                                nickname->clear();
                                socket->abort();
                                socket->connectToHost(ipMayChu->text(), congMayChu->value());
                                nickname->insert(name); khungSoanThao->insert("/checkname");
                                anNutGuiTin();
                                nhanDuLieu();


                            }


                    }
                    else {
                        bool ok = false;
                            QString Password = QInputDialog::getText(this, "Password"," Nếu bạn là chủ phòng,hãy nhập Password ?", QLineEdit::Normal, QString(), &ok);
                            if (ok ){
                                nickname->clear();
                                socket->abort();
                                socket->connectToHost(ipMayChu->text(), congMayChu->value());



                            }
                    }
}

// Gui tin den may chu
void CuaSoNguoiDung::anNutGuiTin() {
    QByteArray goiTin;
    QDataStream out(&goiTin, QIODevice::WriteOnly);
    QTime ct = QTime::currentTime();


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
int thoat=0;
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
    if (tinNhan=="3")
    {
       thoat =1;
        QMessageBox::information(this,"Nickname", "Tên của bạn đã được sử dụng, vui lòng thử lại. ");
        kichThuoc = 0; nickname->clear(); nickname->setReadOnly(true);
        khungSoanThao->setReadOnly(true);
        ngatKetNoi();
    }
   else{

        if (tinNhan=="2")
        {
            QMessageBox::information(this,"Thông báo", "Bạn đã được kết nối tới máy chủ.");
       kichThuoc = 0;nickname->setReadOnly(true);khungSoanThao->setReadOnly(false);}
    else{ if(tinNhan=="5")
            {QMessageBox::information(this,"Password", "Xin chào Owner !");
                kichThuoc = 0;nickname->setReadOnly(true);khungSoanThao->setReadOnly(false);}
            else {if (tinNhan=="6")
            {
                       QMessageBox::critical(this, "Password", "Sai mật khẩu, xin thử lại.");
                       nickname->clear();
                       nickname->setReadOnly(true);khungSoanThao->setReadOnly(true);
                ngatKetNoi();}
                else { if (tinNhan=="1")
                    { thoat=1;

                      QMessageBox::information(this,"Thông báo", "Phòng chat chưa được tạo, hãy tạo phòng.");
                      kichThuoc=0;nickname->clear();
                      nickname->setReadOnly(true); khungSoanThao->setReadOnly(true);
                      ngatKetNoi();

                    }
                    else {if (tinNhan=="4"){
                            QMessageBox::critical(this, "Thông báo", "Phòng chat đã có chủ,vui lòng chọn lại");
                            nickname->clear();
                            nickname->setReadOnly(true);khungSoanThao->setReadOnly(true);
                     ngatKetNoi();}
                        else { if(tinNhan.indexOf("7")==0){
                            QMessageBox::information(this,"Thông báo", "Tên của bạn đã được thay đổi thành công");
                            tinNhan.remove(0,1);
                            nickname->clear();
                            nickname->insert(tinNhan);
                            nickname->setReadOnly(true);}
                            else {


                        cuocHoiThoai->append(tinNhan);

                // Dat lai kich thuoc la 0 de cho tin nhan tiep theo
                kichThuoc = 0;}}}
                    }}}}

//            qDebug() << thoat;
//            qDebug() << tinNhan;

}


void CuaSoNguoiDung::ketNoi() {
    cuocHoiThoai->append(tr("<em>Kết nối thành công !</em>"));
    nutKetNoi->setEnabled(false); nutGuiTin->setEnabled(true);
}


void CuaSoNguoiDung::ngatKetNoi() {
    cuocHoiThoai->append(tr("<em>Tạm biệt, hẹn gặp lại sau !</em>"));
    nutKetNoi->setEnabled(true);
    nutGuiTin->setEnabled(false);
}


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

    nutKetNoi->setEnabled(true);  nutGuiTin->setEnabled(false);



}

