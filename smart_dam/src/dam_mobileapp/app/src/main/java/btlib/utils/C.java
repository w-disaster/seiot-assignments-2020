package btlib.utils;

public class C {
    public static final String LIB_TAG = "BluetoothLib";
    public static final String APP_LOG_TAG = "BT CLN";

    public class channel {
        public static final int MESSAGE_RECEIVED = 0;
        public static final int MESSAGE_SENT = 1;
    }

    public class message {
        public static final char MESSAGE_TERMINATOR = '\n';
    }

    public class emulator {
        public static final String HOST_IP = "10.0.2.2";
        public static final int HOST_PORT = 8080;
    }

    public class bluetooth {
        public static final int ENABLE_BT_REQUEST = 1;
        public static final String BT_DEVICE_ACTING_AS_SERVER_NAME = "isi00"; //MODIFICARE QUESTA COSTANTE CON IL NOME DEL DEVICE CHE FUNGE DA SERVER
        public static final String BT_SERVER_UUID = "7ba55836-01eb-11e9-8eb2-f2801f1b9fd1";
    }

}
