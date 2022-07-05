package com.example.mobile_teamproject_bundle;

import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.widget.RelativeLayout;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;

public class DietRecommendService extends Service {
    public static final String TAG = RecommendDietList.class.getCanonicalName();
    private NetworkConnectionStateMonitor networkConnectionStateMonitor;

    private Context mContext;
    private int pageNo = 1;
    private int currentItemSize = 0;
    private String mainCategoryName;
    private String dietSeCode;

    private RelativeLayout addItemButton;
    //private RecommendDietListAdapter adapter;

    private DataManager dataManager = new DataManager();

    public DietRecommendService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        mContext = this;

        if (networkConnectionStateMonitor == null) {
            networkConnectionStateMonitor = new NetworkConnectionStateMonitor(mContext);
            networkConnectionStateMonitor.register();
        }

        if (networkConnectionStateMonitor != null) {
            init();
            setRecommendDietListResponse();
        }
        return super.onStartCommand(intent, flags, startId);
    }
    private void init() {

        dietSeCode = PreferenceManager.getString(mContext, "dietSeCode");
        mainCategoryName = PreferenceManager.getString(mContext, "mainCategoryName");
    }

    private void setRecommendDietListResponse() {
        // Get Api response
        new Thread(new Runnable() {
            @Override
            public void run() {
                //Log.d("1service","start");
                getRecommendDietListResponse();
                int num = dataManager.getRecommendDietListResponses().size();
                int position = (int) (Math.random()*(num-1));  //0~num
                PreferenceManager.setString(mContext, "cntntsNo", String.valueOf(dataManager.getRecommendDietListResponses().get(position).getCntntsNo()));
                PreferenceManager.setString(mContext,"mainCategoryName", mainCategoryName);
                //Intent intent = new Intent(getApplicationContext(), Diet_Recommend.class);
                //intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                //startActivity(intent);
                //Log.d("1service","ENd");
                startService(new Intent(getApplicationContext(), RecommendCustomDietService.class));
            }
        }).start();
    }

    private void getRecommendDietListResponse() {
        String apiKey = "20210430IQ6XOLUFMGFHTTQEMNG1A";
        String apiUrl = "http://api.nongsaro.go.kr/service/recomendDiet/recomendDietList?apiKey=" + apiKey
                + "&dietSeCode=" + dietSeCode + "&pageNo=" + pageNo;

        try {
            URL url = new URL(apiUrl);
            InputStream inputStream = url.openStream();

            XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
            XmlPullParser xpp = factory.newPullParser();
            xpp.setInput(new InputStreamReader(inputStream, "UTF-8"));

            int eventType = xpp.getEventType();
            String startTag = "";
            boolean isItemType = false;
            while (eventType != XmlPullParser.END_DOCUMENT) {
                if (eventType == XmlPullParser.START_DOCUMENT) {
                    // XML 데이터 시작
                } else if (eventType == XmlPullParser.START_TAG) {
                    startTag = xpp.getName();
                    if (startTag.equals("item")) {
                        isItemType = true;
                        dataManager.getRecommendDietListResponses().add(new RecommendDietListResponse());
                    }
                } else if (eventType == XmlPullParser.TEXT) {
                    if (isItemType) {
                        if (startTag.equals("cntntsNo")) {
                            dataManager.getLastDietListData().setCntntsNo(Integer.parseInt(xpp.getText()));
                        } else if (startTag.equals("dietNm")) {
                            dataManager.getLastDietListData().setDietNm(xpp.getText());
                        } else if (startTag.equals("fdNm")) {

                        } else if (startTag.equals("cntntsSj")) {

                        } else if (startTag.equals("cntntsChargerEsntlNm")) {

                        } else if (startTag.equals("registDt")) {

                        } else if (startTag.equals("cntntsRdcnt")) {

                        } else if (startTag.equals("rtnFileSeCode")) {

                        } else if (startTag.equals("rtnFileSn")) {

                        } else if (startTag.equals("rtnStreFileNu")) {

                        } else if (startTag.equals("rtnImageDc")) {
                            dataManager.getLastDietListData().setRtnImageDc(xpp.getText());
                        } else if (startTag.equals("rtnThumbFileNm")) {

                        } else if (startTag.equals("rtnImgSeCode")) {

                        }
                    }
                } else if (eventType == XmlPullParser.END_TAG) {
                    String endTag = xpp.getName() ;
                    if (endTag.equals("item")) {
                        startTag = "";
                        isItemType = false;
                    }
                }
                eventType = xpp.next();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    @Override
    public void onDestroy() {
        super.onDestroy();
        networkConnectionStateMonitor.unRegister();
    }
}