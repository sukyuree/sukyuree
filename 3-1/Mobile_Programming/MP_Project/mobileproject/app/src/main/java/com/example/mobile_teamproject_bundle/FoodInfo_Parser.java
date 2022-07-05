package com.example.mobile_teamproject_bundle;

import android.util.Log;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;

public class FoodInfo_Parser {
    ArrayList<PriceDataStruct> m_xmlData;

    public FoodInfo_Parser() {
    }

    public ArrayList<PriceDataStruct> GetXmlData(InputStream line, String searchTxt) {
        this.m_xmlData = new ArrayList();
        PriceDataStruct xmlData = new PriceDataStruct();
        String m_searchTxt = "";

        try {
            m_searchTxt = URLEncoder.encode(searchTxt, "UTF8");
        } catch (UnsupportedEncodingException var12) {
            var12.printStackTrace();
        }

        try {
            boolean startItemTag = false;
            String tagName = "";
            XmlPullParserFactory parserCreator = null;
            parserCreator = XmlPullParserFactory.newInstance();
            XmlPullParser parser = parserCreator.newPullParser();
            /*BufferedReader in = new BufferedReader(new InputStreamReader(
                    url.openStream(), "UTF-8"));*/
            parser.setInput(line, null);

            for (int eventType = parser.getEventType(); eventType != 1; eventType = parser.next()) {
                switch (eventType) {
                    case 0:
                    case 1:
                    default:
                        break;
                    case 2:
                        tagName = parser.getName();
                        //Log.d("tagName",tagName);
                        if (tagName.equals("item")) {
                            startItemTag = true;
                            xmlData = new PriceDataStruct();
                        }
                        break;
                    case 3:
                        tagName = parser.getName();
                        if (tagName.equals("item")) {
                            startItemTag = false;
                            this.m_xmlData.add(xmlData);
                        }
                        break;
                    case 4:
                        if (startItemTag) {
                            if (tagName.equals("total")) {
                                xmlData.setTotal(Integer.parseInt(parser.getText()));
                            } else if (tagName.equals("start")) {
                                xmlData.setStart(Integer.parseInt(parser.getText()));
                            } else if (tagName.equals("display")) {
                                xmlData.setDisplay(Integer.parseInt(parser.getText()));
                            } else if (tagName.equals("title")) {
                                //Log.d("Title",parser.getText());
                                xmlData.setTitle(parser.getText());
                            } else if (tagName.equals("link")) {
                                xmlData.setLink(parser.getText());
                            } else if (tagName.equals("image")) {
                                xmlData.setImage(parser.getText());
                            } else if (tagName.equals("lprice")) {
                                xmlData.setLprice(Integer.parseInt(parser.getText()));
                            } else if (tagName.equals("hprice")) {
                                xmlData.setHprice(Integer.parseInt(parser.getText()));
                            } else if (tagName.equals("mallName")) {
                                xmlData.setMallName(parser.getText());
                            } else if (tagName.equals("productId")) {
                                xmlData.setProductId(Long.parseLong(parser.getText()));
                            }
                        }
                }
            }
        } catch (Exception var13) {
            Log.d("api error", var13.getMessage());
        }
        this.m_xmlData.add(xmlData);
        return this.m_xmlData;
    }
}
