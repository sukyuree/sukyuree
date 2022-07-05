package com.example.mobile_teamproject_bundle;

import android.app.AlertDialog;
import android.app.Service;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.IBinder;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;

public class SelectDiet extends Service {
    private Context mContext;
    private NetworkConnectionStateMonitor networkConnectionStateMonitor;

    private ArrayList<String> dietSeCode = new ArrayList<>();

    public SelectDiet() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    public int onStartCommand(Intent intent, int flags, int startId){
        init();
        if (networkConnectionStateMonitor != null) {
            getMainCategoryTitleResponse();
        }
        return super.onStartCommand(intent, flags, startId);
    }
    private void init() {
        mContext = this;

        if (networkConnectionStateMonitor == null) {
            networkConnectionStateMonitor = new NetworkConnectionStateMonitor(mContext);
            networkConnectionStateMonitor.register();


            //TODO: Add progress dialog (or bar) after first release
            if (!networkConnectionStateMonitor.isConnected()) {

                final AlertDialog alertDialog = new AlertDialog.Builder(mContext)
                        .setCancelable(false)
                        .setMessage(R.string.dialog_network_alert_message)
                        .setPositiveButton("확인", null)
                        .create();

                alertDialog.setOnShowListener(new DialogInterface.OnShowListener() {
                    @Override
                    public void onShow(DialogInterface dialog) {
                        Button positiveButton = alertDialog.getButton(AlertDialog.BUTTON_POSITIVE);
                        positiveButton.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                if (networkConnectionStateMonitor.isConnected()) {
                                    alertDialog.dismiss();
                                    Toast.makeText(mContext, R.string.network_connected_message, Toast.LENGTH_SHORT).show();
                                } else {
                                    Toast.makeText(mContext, R.string.network_unconnected_message, Toast.LENGTH_SHORT).show();
                                }
                            }
                        });
                    }
                });

                alertDialog.show();
            }
        }

    }

    private void getMainCategoryTitleResponse() {
        // Get Api response
        new Thread(new Runnable() {
            @Override
            public void run() {
                getMainCategoryResponse();
                onClickCategory();
            }
        }).start();
    }

    private void getMainCategoryResponse() {

        dietSeCode.clear();

        String apiKey = "20210430IQ6XOLUFMGFHTTQEMNG1A";

        String apiUrl = "http://api.nongsaro.go.kr/service/recomendDiet/mainCategoryList?apiKey=" + apiKey;


        try {
            URL url = new URL(apiUrl);
            URLConnection urlcon = url.openConnection();
            InputStream inputStream = urlcon.getInputStream();
            XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
            XmlPullParser xpp = factory.newPullParser();
            xpp.setInput(new InputStreamReader(inputStream, "UTF-8"));

            int eventType = xpp.getEventType();
            while (eventType != XmlPullParser.END_DOCUMENT) {
                switch (eventType) {
                    case XmlPullParser.START_TAG:
                        if (xpp.getName().equals("item")) { // fist research result

                        } else if (xpp.getName().equals("dietSeCode")) {
                            xpp.next();
                            dietSeCode.add(xpp.getText());
                            //Log.d("xpp",xpp.getText());
                        }
                        break;
                }
                eventType = xpp.next();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void onClickCategory() {

        int rand = (int) (Math.random() * 4);
        Intent intent;

        switch(rand) {
            case 0:
                PreferenceManager.setString(mContext, "dietSeCode", dietSeCode.get(0));
                PreferenceManager.setString(mContext, "mainCategoryName", getString(R.string.study_diet_main_category_name));
                startService(new Intent(this, DietRecommendService.class));
                //intent = new Intent(getApplicationContext(), RecommendDietListActivity.class);
                //startActivity(intent);
                break;
            case 1:
                PreferenceManager.setString(mContext, "dietSeCode", dietSeCode.get(1));
                PreferenceManager.setString(mContext, "mainCategoryName", getString(R.string.healthy_diet_main_category_name));
                startService(new Intent(this, DietRecommendService.class));
                //intent = new Intent(getApplicationContext(), RecommendDietListActivity.class);
                //startActivity(intent);
                break;
            case 2:
                PreferenceManager.setString(mContext, "dietSeCode", dietSeCode.get(2));
                PreferenceManager.setString(mContext, "mainCategoryName", getString(R.string.home_meal_main_category_name));
                startService(new Intent(this, DietRecommendService.class));
                //intent = new Intent(getApplicationContext(), RecommendDietListActivity.class);
                //startActivity(intent);
                break;
            case 3:
                PreferenceManager.setString(mContext, "dietSeCode", dietSeCode.get(3));
                PreferenceManager.setString(mContext, "mainCategoryName", getString(R.string.event_diet_main_category_name));
                startService(new Intent(this, DietRecommendService.class));
                //intent = new Intent(getApplicationContext(), RecommendDietListActivity.class);
                //startActivity(intent);
                break;
            case 4:
                PreferenceManager.setString(mContext, "dietSeCode", dietSeCode.get(4));
                PreferenceManager.setString(mContext, "mainCategoryName", getString(R.string.refresh_diet_main_category_name));
                startService(new Intent(this, DietRecommendService.class));
                //intent = new Intent(getApplicationContext(), RecommendDietListActivity.class);
                //startActivity(intent);
                break;
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        networkConnectionStateMonitor.unRegister();
    }
}