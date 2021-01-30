#include "cxmlparameter.h"
#include<QDebug>
#include<QTextCodec>

CXMLParameter::CXMLParameter(QString file)
{
    filename = file;
    rootname_list.clear();
    firstnodename_list.clear();
    groupname_list.clear();
    keyname_list.clear();
    typevalue_list.clear();
    minvalue_list.clear();
    maxvalue_list.clear();
    chinaname_list.clear();
    describe_list.clear();
    reboot_list.clear();
    speed_list.clear();
}
//<?xml version="1.0" encoding="UTF-8"?>
//<COMMAND>
//    <OBJECT>USER</OBJECT>
//    <ACTION>LOGIN</ACTION>
//    <DATA>
//        <USER NAME="root" PASSWORD="123456"/>
//    </DATA>
//</COMMAND>
int CXMLParameter::writeXml()
{
    QFile file(filename);
     if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
         return -2;
     QTextStream out(&file);
     QDomDocument doc;
     QDomText text;
     QDomElement element;
     QDomAttr attr;
     QDomProcessingInstruction instruction;
     instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
     doc.appendChild( instruction );

     QDomElement root = doc.createElement( "COMMAND" );
     doc.appendChild(root);
     element = doc.createElement( "OBJECT" );
     text = doc.createTextNode( "USER" );
     element.appendChild(text);
     root.appendChild(element);

     element = doc.createElement( "ACTION" );
     text = doc.createTextNode( "LOGIN" );
     element.appendChild(text);
     root.appendChild(element);

     element = doc.createElement( "DATA" );
     root.appendChild(element);

     QDomElement userElement = doc.createElement( "USERINFO" );
     attr = doc.createAttribute( "NAME" );
     attr.setValue("root");
     userElement.setAttributeNode(attr);
     attr = doc.createAttribute( "PASSWORD" );
     attr.setValue("123456");
     userElement.setAttributeNode(attr);
     element.appendChild(userElement);

     doc.save(out, 4);       //each line space of file is 4
     return 0;
}

int CXMLParameter::readXml()
{
    QDomDocument doc;
    QFile file(filename);
    QString error = "";
    int row = 0, column = 0;
//    qDebug() <<filename;
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "fail to open file!";
        return -2;
    }

    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();
    while(!node.isNull()) {
       QDomElement element = node.toElement(); // try to convert the node to an element.
       if(!element.isNull()) {
//          qDebug()<<element.tagName() << "NAME=" << element.attributeNode("NAME").value();
          QString name = element.attributeNode("NAME").value();
          //qDebug()<<name;
          rootname_list.append(name);
          QDomNode nodeson = element.firstChild();
          while(!nodeson.isNull()) {
              QDomElement elementson = nodeson.toElement();
              if(!elementson.isNull()) {
//                  qDebug()<< "---" <<elementson.tagName();
                  name = elementson.tagName();
                  firstnodename_list.append(name);
                  if (elementson.hasAttribute("GROUP_NAME")) {
//                      qDebug()<< "---" << "GROUP_NAME=" << elementson.attributeNode("GROUP_NAME").value();
                      name = elementson.attributeNode("GROUP_NAME").value();
                      groupname_list.append(name);
                  }
                  if (elementson.hasAttribute("KEY_NAME")) {
//                      qDebug()<< "---" << "KEY_NAME=" << elementson.attributeNode("KEY_NAME").value();
                      name = elementson.attributeNode("KEY_NAME").value();
                      keyname_list.append(name);
                  }
                  if (elementson.hasAttribute("TYPE_VALUE")) {
//                      qDebug()<< "---" << "TYPE_VALUE=" << elementson.attributeNode("TYPE_VALUE").value();
                      name = elementson.attributeNode("TYPE_VALUE").value();
                      typevalue_list.append(name);
                  }
                  if (elementson.hasAttribute("MIN_VALUE")) {
//                      qDebug()<< "---" << "MIN_VALUE=" << elementson.attributeNode("MIN_VALUE").value();
                      name = elementson.attributeNode("MIN_VALUE").value();
                      minvalue_list.append(name);
                  }
                  if (elementson.hasAttribute("MAX_VALUE")) {
//                      qDebug()<< "---" << "MAX_VALUE=" << elementson.attributeNode("MAX_VALUE").value();
                      name = elementson.attributeNode("MAX_VALUE").value();
                      maxvalue_list.append(name);
                  }
                  if (elementson.hasAttribute("NAME")) {
//                      qDebug()<< "---" << "NAME=" << elementson.attributeNode("NAME").value();
                      name = elementson.attributeNode("NAME").value();
                      chinaname_list.append(name);
                  }
                  if (elementson.hasAttribute("DESCRIBE")) {
//                      qDebug()<< "---" << "DESCRIBE=" << elementson.attributeNode("DESCRIBE").value();
                      name = elementson.attributeNode("DESCRIBE").value();
                      describe_list.append(name);
                  }
                  if (elementson.hasAttribute("REBOOT")) {
//                      qDebug()<< "---" << "REBOOT=" << elementson.attributeNode("REBOOT").value();
                      name = elementson.attributeNode("REBOOT").value();
                      reboot_list.append(name);
                  }
                  if (elementson.hasAttribute("SPEED")) {
//                      qDebug()<< "---" << "SPEED=" << elementson.attributeNode("SPEED").value();
                      name = elementson.attributeNode("SPEED").value();
                      speed_list.append(name);
                  }
              }
              nodeson = nodeson.nextSibling();
          }
       }
       node = node.nextSibling();
    }
    return 0;
}
int CXMLParameter::readXml2()
{
    QDomDocument doc;
    QFile file(filename);
    QString error = "";
    int row = 0, column = 0;
    if (!file.open(QIODevice::ReadOnly)) return -2;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChildElement();
    while(!node.isNull()) {
       QDomElement element = node.toElement(); // try to convert the node to an element.
       if(!element.isNull()) {
           if (element.tagName() == "DATA") {
               qDebug()<< "---" <<element.tagName();
                QDomNodeList list = element.childNodes();
                for(int index = 0; index < list.count(); index++) {
                    QDomNode list_node = list.item(index);
                    QDomElement list_element = list_node.toElement();
                    if (list_element.hasAttribute("NAME")) {
                        qDebug()<< "---" << "NAME =" << list_element.attributeNode("NAME").value();
                    }
                    if (list_element.hasAttribute("PASSWORD")) {
                        qDebug()<< "---" << "PASSWORD =" << list_element.attributeNode("PASSWORD").value();
                    }
                    if (list_element.hasAttribute("PASSWORD")) {
                        qDebug()<< "---" << "PASSWORD =" << list_element.attributeNode("PASSWORD").value();
                    }
                }
           }
           else {
               qDebug()<<element.tagName() << ":" << element.text();
           }
       }
       node = node.nextSibling();
    }
    return 0;
}

//增加xml内容
void CXMLParameter::AddXml()
{
    //打开文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //增加一个一级子节点以及元素
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomElement book=doc.createElement("book");
    book.setAttribute("id",3);
    book.setAttribute("time","1813/1/27");
    QDomElement title=doc.createElement("title");
    QDomText text;
    text=doc.createTextNode("Pride and Prejudice");
    title.appendChild(text);
    book.appendChild(title);
    QDomElement author=doc.createElement("author");
    text=doc.createTextNode("Jane Austen");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

//删减xml内容
void CXMLParameter::RemoveXml()
{
    //打开文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //删除一个一级子节点及其元素，外层节点删除内层节点于此相同
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();  //一定要记得关掉啊，不然无法完成操作

    QDomElement root=doc.documentElement();
    QDomNodeList list=doc.elementsByTagName("book"); //由标签名定位
    for(int i=0;i<list.count();i++)
    {
        QDomElement e=list.at(i).toElement();
        if(e.attribute("time")=="2007/5/25")  //以属性名定位，类似于hash的方式，warning：这里仅仅删除一个节点，其实可以加个break
            root.removeChild(list.at(i));
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

//更新xml内容
void CXMLParameter::UpdateXml()
{
    //打开文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomNodeList list=root.elementsByTagName("book");
    QDomNode node=list.at(list.size()-1).firstChild(); //定位到第三个一级子节点的子元素
    QDomNode oldnode=node.firstChild(); //标签之间的内容作为节点的子节点出现,当前是Pride and Projudice
    node.firstChild().setNodeValue("Emma");
    QDomNode newnode=node.firstChild();
    node.replaceChild(newnode,oldnode);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}
