#ifndef __MSG__
#define __MSG__


class Msg{

  String content;
  bool isReady;
  
  public:
    String getContent();
    void setContent(String content);
    void setMsgReady(bool flag);
    bool isMsgReady();
};

#endif
