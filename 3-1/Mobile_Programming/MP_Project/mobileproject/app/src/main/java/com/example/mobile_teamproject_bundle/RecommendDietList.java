package com.example.mobile_teamproject_bundle;

// Request API
public class RecommendDietList {
    // required
    private String apiKey; // 발급받은 Open API 인증키
    private String dietSeCode; // 식단 구분코드

    //optional
    private int pageNo; // 조회할 페이지 번호
    private int numOfRows; // 한 페이지에 제공할 건수

    public String getApiKey() {
        return apiKey;
    }

    public void setApiKey(String apiKey) {
        this.apiKey = apiKey;
    }

    public String getDietSeCode() {
        return dietSeCode;
    }

    public void setDietSeCode(String dietSeCode) {
        this.dietSeCode = dietSeCode;
    }

    public int getPageNo() {
        return pageNo;
    }

    public void setPageNo(int pageNo) {
        this.pageNo = pageNo;
    }

    public int getNumOfRows() {
        return numOfRows;
    }

    public void setNumOfRows(int numOfRows) {
        this.numOfRows = numOfRows;
    }
}
