package com.dm.dam_mobile;

public enum State {
    NORMAL("NORMAL"),
    PRE_ALARM("PRE ALARM"),
    ALARM("ALARM"),
    CONNECTED("CONNECTED"),
    DISCONNECTED("DISCONNECTED");

    private String text;

    State(final String text){
        this.text = text;
    }

    public String getText(){
        return this.text;
    }
}
