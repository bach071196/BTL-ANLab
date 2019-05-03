#include "CuaSoMayChu.h"
QString const password = "123456";
QString startTime;
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
    if (!mayChu->listen(QHostAddress("127.0.0.1"), 1080)) { // Khoi dong may chu o cong 1080 và tren tat ca cac dia chi IP cua may
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
    nguoiDungHt = nguoiDungMoi;
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


    QList<QString> command;

    command << "checkname" << "reportall" << "reportonline"<< "reportoffline"<< "info"<< "setinfo"<< "leave"<< "nickname"<< "setnick"<< "ban"<< "unban"<< "mods"<< "mod"<< "unmod"<<"fillters"<< "fillter"<< "unfillter"<<"checkpass";
    in >> tinNhan;
    name = tinNhan;
    name = name.remove(0,9);
    name = name.remove(name.indexOf(" "),(name.size()-name.indexOf(" ")));
    if  (tinNhan.indexOf(QString("/"))== (12+name.size()))
    {   QString checkCommand;
        QString checkInfoCommand;
        QString mod;
        QString infor = "Tên người tạo: Owner" ;
        QString setInfo;
        QTcpSocket *setTcp;
        QString word;
        QString replaced_word;
        QString offline;

        kichThuoc = 0;
        checkCommand=tinNhan;
        checkCommand=checkCommand.remove(0,tinNhan.indexOf("/")+1);
                    checkInfoCommand = checkCommand;
                        if (checkCommand.indexOf(" ")!=-1)
                        {
                            checkCommand = checkCommand.remove(checkCommand.indexOf(" "),(checkCommand.size()-checkCommand.indexOf(" ")));
                            checkInfoCommand = checkInfoCommand.remove(0, checkInfoCommand.indexOf(" ")+1);
                        }
                        else
                        {
                            checkInfoCommand.clear();}

qDebug()<< checkCommand;
qDebug() << checkInfoCommand;

   switch (command.indexOf(checkCommand)) {

    case  0: //checkname
       if( infoNguoiDung.values().contains("Admin")== false)
       {
           guiTinNhanChoNguoiHt("1"); kichThuoc=0;
       }
       else if (infoNguoiDung.keys().contains(name)== false)
           {
            infoNguoiDung.insert(name,"User");
            infoTcp.insert(name,nguoiDungHt);
            dsonline << name;

            guiTinNhanChoNguoiHt("2");
            kichThuoc = 0;
           }

                else
                {
                                guiTinNhanChoNguoiHt("3");

                                kichThuoc = 0;

                }
        break;
    case  17: //checkpass
       if (dsonline.contains("Owner")==true)
       {

           guiTinNhanChoNguoiHt("4");
           kichThuoc=0;


       }
       else {if (checkInfoCommand==password)
                       {

                      guiTinNhanChoNguoiHt("5"); kichThuoc=0;
                    startTime= tinNhan.remove(9,tinNhan.size()-9);
                    chuphong = nguoiDungHt;
                    infoNguoiDung.insert("Owner","Admin");
                    infoTcp.insert("Owner",nguoiDungHt);
                    dsonline << name;}
                    else
                    {
//
                        guiTinNhanChoNguoiHt("6");

                        kichThuoc = 0;

           }}
        break;
    case  1: //reportall
        if (infoNguoiDung.value(name)=="Admin"||infoNguoiDung.value(name)=="Mod"){
                  QString reportall;
                  for(int i=0; i<(infoNguoiDung.keys().size()); i++)
                 {reportall+= (infoNguoiDung.keys()[i]);
                      reportall+= "  ";}
                  nguoiDungHt = infoTcp.value(name);
               guiTinNhanChoNguoiHt(" danh sách all:" + reportall); kichThuoc = 0;
                }
              else
              {
            nguoiDungHt = infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                    }

        break;
    case  2: //reportonline
        if (infoNguoiDung.value(name)=="Admin"||infoNguoiDung.value(name)=="Mod")
              {
            QString dsonl;
            nguoiDungHt = infoTcp.value(name);
            dsonl = "Danh sách online:";

                  for(int i=0; i<(dsonline.size()); i++)
            {dsonl+= dsonline[i]; dsonl+= "  ";}
                  guiTinNhanChoNguoiHt(dsonl);
                   kichThuoc = 0;}
              else
              {
            nguoiDungHt = infoTcp.value(name);guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }
        break;
    case  3: //reportoffline
       for(int i=0; i<infoNguoiDung.keys().size(); i++)
      {
       if (dsonline.contains(infoNguoiDung.keys()[i])==false)
       {dsoffline << infoNguoiDung.keys()[i];
           }}

       offline= "Danh sach offline:  ";
       for(int i=0; i<dsoffline.size(); i++)
       {
       offline+= dsoffline[i];
       offline+= "  ";}
       nguoiDungHt = infoTcp.value(name);
       guiTinNhanChoNguoiHt(offline);
       kichThuoc=0;

        break;
    case  4: // info

                           infor+= "Giờ tạo:";
                           infor+= startTime ;
                           infor+= "Luật lệ tham gia:" ;
                           infor+= content ;
                  nguoiDungHt = infoTcp.value(name);
              guiTinNhanChoNguoiHt(infor); kichThuoc = 0;
        break;
    case  5: //setinfo
        if (infoNguoiDung.value(name)=="Admin")
              {content=checkInfoCommand;}
              else
              { nguoiDungHt = infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }
        break;
    case  6: //leave
ngatKetNoi();

        break;
    case  7: //nickname
        nguoiDungHt = infoTcp.value(name);
        guiTinNhanChoNguoiHt("Tên của bạn là:" + name); kichThuoc = 0;
        break;
    case  8: //setnick

        setInfo=infoNguoiDung.value(name);
        infoNguoiDung.remove(name); infoNguoiDung.insert(checkInfoCommand,setInfo);
        setTcp = infoTcp.value(name);
        infoTcp.remove(name); infoTcp.insert(checkInfoCommand,setTcp);
        dsonline.removeOne(name); dsonline << checkInfoCommand;

        name=checkInfoCommand;
        nguoiDungHt = infoTcp.value(name); checkInfoCommand="7" + checkInfoCommand;
        guiTinNhanChoNguoiHt(checkInfoCommand); kichThuoc= 0;
        break;
    case  9: //ban
        if (infoNguoiDung.value(name)=="Admin")
              {
            nguoiDungHt= infoTcp.value(name);
                               guiTinNhanChoNguoiHt("Đã cấm chat người dùng thành công"); kichThuoc = 0;
                               banlist << checkInfoCommand;
        }
              else
                  {nguoiDungHt= infoTcp.value(name);
                   guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }

        break;
    case  10: //unban
        if (infoNguoiDung.value(name)=="Admin")
              {banlist.removeOne(checkInfoCommand);
            nguoiDungHt= infoTcp.value(name);
                               guiTinNhanChoNguoiHt("Đã hủy ban người dùng"); kichThuoc = 0;
        }
              else
                 {
                    nguoiDungHt= infoTcp.value(name);
                    guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                  }

        break;
    case  11: //mods
          mod= "Chủ phòng: Owner, Danh sách mod:  " ;
          for(int i=0; i<modlist.size(); i++)
          {mod+=modlist[i];
           mod+="  ";}
           nguoiDungHt= infoTcp.value(name);
             guiTinNhanChoNguoiHt(mod ); kichThuoc = 0;
        break;
    case  12: //mod
        if (infoNguoiDung.value(name)=="Admin")
              {infoNguoiDung.remove(checkInfoCommand);
            infoNguoiDung.insert(checkInfoCommand,"Mod");
            modlist << checkInfoCommand;
            nguoiDungHt= infoTcp.value(name);
                        guiTinNhanChoNguoiHt("Đã cấp quyền mod cho người dùng."); kichThuoc = 0;}
              else
              {nguoiDungHt= infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }
        break;
    case  13: //unmod
        if (infoNguoiDung.value(name)=="Admin")
              {infoNguoiDung.remove(checkInfoCommand);
            infoNguoiDung.insert(checkInfoCommand,"User");
            modlist.removeOne(checkInfoCommand);
            nguoiDungHt= infoTcp.value(name);
                        guiTinNhanChoNguoiHt("Đã hủy quyền mod của người dùng."); kichThuoc = 0;}
              else
              {nguoiDungHt= infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }

        break;
    case  15: //fillter
        word = checkInfoCommand.remove(checkInfoCommand.indexOf(" "),(checkInfoCommand.size()-checkInfoCommand.indexOf(" ")));
        replaced_word = checkInfoCommand.remove(0, checkInfoCommand.indexOf(" ")+1);

        if (infoNguoiDung.value(name)=="Admin"||infoNguoiDung.value(name)=="Mod")
              {fillterList.insert(word,replaced_word );
        nguoiDungHt=infoTcp.value(name);
        guiTinNhanChoNguoiHt("Đã thêm từ khóa vào danh sách lọc"); kichThuoc=0;}
              else
                  {nguoiDungHt=infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }
        break;
    case  14: //fillters
        if (infoNguoiDung.value(name)=="Admin"||infoNguoiDung.value(name)=="Mod")
         {
            QString fillters;
       fillters = "Danh sach tu khoa thay the";


       for(int i=0; i<fillterList.keys().size(); i++)
    { fillters+= fillterList.keys()[i];
           fillters+=fillterList.values()[i];
       }
           nguoiDungHt=infoTcp.value(name);
     guiTinNhanChoNguoiHt(fillters); kichThuoc = 0;

     }

    else
              {
            nguoiDungHt=infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }
        break;
    case  16: //unfillter
        word = checkInfoCommand.remove(checkInfoCommand.indexOf(" "),(checkInfoCommand.size()-checkInfoCommand.indexOf(" ")));
        replaced_word = checkInfoCommand.remove(0, checkInfoCommand.indexOf(" ")+1);
        if (infoNguoiDung.value(name)=="Admin"||infoNguoiDung.value(name)=="Mod")
              {fillterList.remove(word);
        nguoiDungHt=infoTcp.value(name);
        guiTinNhanChoNguoiHt("Đã loại từ khóa khỏi danh sách lọc"); kichThuoc=0;}
              else
                  {
            nguoiDungHt=infoTcp.value(name);
            guiTinNhanChoNguoiHt("Bạn không được phép sử dụng lệnh này"); kichThuoc = 0;
                   }


        break;

    default:
        nguoiDungHt=infoTcp.value(name);
        guiTinNhanChoNguoiHt("Lệnh không hợp lệ, xin thử lại."); kichThuoc=0;
        break;
   }



   }
    else
    {if  (tinNhan.indexOf(QString("@"))== (12+name.size()))

            {   QString checkCommand;
                QString checkInfoCommand;

                kichThuoc = 0;
                checkCommand=tinNhan;
                checkCommand=checkCommand.remove(0,tinNhan.indexOf("@")+1);
                            checkInfoCommand = checkCommand;
                                if (checkCommand.indexOf(" ")!=-1)
                                {
                                    checkCommand = checkCommand.remove(checkCommand.indexOf(" "),(checkCommand.size()-checkCommand.indexOf(" ")));
                                    checkInfoCommand = checkInfoCommand.remove(0, checkInfoCommand.indexOf(" ")+1);
                                }
                                else
                                {
                                    checkInfoCommand.clear();}
                if (infoNguoiDung.value(name)=="Admin"||infoNguoiDung.value(name)=="Mod"&&(dsonline.contains(checkCommand)==true))
                                {  QString tnrieng;
                                    nguoidungbatky= infoTcp.value(checkCommand);
                                    nguoiDungHt= infoTcp.value(name);
                                    tnrieng= name;
                                    tnrieng+= " : ";
                                    tnrieng+= checkInfoCommand;
                                    guiTinNhanChoNguoiBatky(tnrieng); kichThuoc=0;
                                    guiTinNhanChoNguoiHt("Tin nhắn riêng đã được gửi"); kichThuoc=0;
                                }
                                    else {
                                    nguoiDungHt= infoTcp.value(name);
                                    guiTinNhanChoNguoiHt("Bạn không được quyền gửi tin hoặc người dùng không tồn tại");
                                    kichThuoc=0;

                                }

        }

        else{
    guiTinNhanChoMoiNguoi(tinNhan);
    kichThuoc = 0;
    }
}
//    if (leave==1)
//    {ngatKetNoi();}


}
void CuaSoMayChu::ngatKetNoi() {
    guiTinNhanChoMoiNguoi(tr("<em>1 người dùng vừa mới rời đi</em>"));

    // Xac dinh xem ai la nguoi ngat ket noi
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Neu khong tim thay nguoi gui tin hieu thi huy bo xu ly
        return;

    nguoiDung.removeOne(socket);
    dsonline.removeOne(infoTcp.key(socket));
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
void CuaSoMayChu::guiTinNhanChoNguoiHt(const QString &tinNhan) {
    // Chuan bi tin nhan gui di
    QByteArray goiTinNhan;
    QDataStream out(&goiTinNhan, QIODevice::WriteOnly);

    out << (quint16) 0; // Viet gia tri 0 o dau goi tin de dat truoc cho de viet kich thuoc tin nhan
    out << tinNhan; // Viet noi dung tin nhan vao goi tin
    out.device()->seek(0); // Quay ve dau goi tin
    out << (quint16) (goiTinNhan.size() - sizeof(quint16)); // Thay 0 bang gia tri kich thuoc that cua tin nhan

     nguoiDungHt->write(goiTinNhan);

                }
void CuaSoMayChu::guiTinNhanChoNguoiBatky( const QString &tinNhan) {
    // Chuan bi tin nhan gui di
    QByteArray goiTinNhan;
    QDataStream out(&goiTinNhan, QIODevice::WriteOnly);

    out << (quint16) 0; // Viet gia tri 0 o dau goi tin de dat truoc cho de viet kich thuoc tin nhan
    out << tinNhan; // Viet noi dung tin nhan vao goi tin
    out.device()->seek(0); // Quay ve dau goi tin
    out << (quint16) (goiTinNhan.size() - sizeof(quint16)); // Thay 0 bang gia tri kich thuoc that cua tin nhan

                nguoidungbatky->write(goiTinNhan);

                }
