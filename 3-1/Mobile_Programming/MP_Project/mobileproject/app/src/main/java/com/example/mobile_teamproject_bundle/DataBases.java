package com.example.mobile_teamproject_bundle;

import android.provider.BaseColumns;

public final class DataBases {

    public static final class CreateDB implements BaseColumns{
        public static final String NAME = "name";
        public static final String AGE = "age";
        public static final String Height = "height";
        public static final String Weight = "weight";
        public static final String Disease = "disease";

        public static final String _TABLENAME0 = "usertable";
        public static final String _CREATE0 = "create table if not exists "+_TABLENAME0+"("
                +_ID+" integer primary key autoincrement, "
                +NAME+" text not null , "
                +AGE+" integer not null , "
                +Height+" text not null );"
                +Weight+" text not null );"
                +Disease+" text not null );";
    }
}
