package com.example.mobile_teamproject_bundle;

import android.content.Context;
import android.content.SharedPreferences;

public class PreferenceManager {
    public static final String DEFAULT_PREFERENCES_NAME = "default_preference_name";

    private static final String DEFAULT_VALUE_STRING = "";

    private static SharedPreferences getDefaultSharedPreferences(Context context) {
        return context.getSharedPreferences(DEFAULT_PREFERENCES_NAME, Context.MODE_PRIVATE);
    }

    /*
     * Store String Value
     *
     * */
    public static void setString(Context context, String key, String value) {
        SharedPreferences prefs = getDefaultSharedPreferences(context);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(key, value);
        editor.commit();
    }

    /*
     * Load String value
     * */
    public static String getString(Context context, String key) {
        SharedPreferences prefs = getDefaultSharedPreferences(context);
        return prefs.getString(key, DEFAULT_VALUE_STRING);
    }
}
