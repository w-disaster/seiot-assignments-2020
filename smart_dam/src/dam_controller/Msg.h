#ifndef __MSG__
#define __MSG__


class Msg{

  String content;
  bool isReady;
  
  public:
    Msg(String content);
    String getContent();
    void setContent(String content);
    void setMsgReady(bool flag);
    bool isMsgReady();
};

#endif
