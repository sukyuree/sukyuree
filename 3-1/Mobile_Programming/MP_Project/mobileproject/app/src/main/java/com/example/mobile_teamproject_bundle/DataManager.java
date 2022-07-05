package com.example.mobile_teamproject_bundle;

import java.util.ArrayList;

public class DataManager {

    ArrayList<RecommendDietListResponse> recommendDietListResponses = new ArrayList<>();

    public ArrayList<RecommendDietListResponse> getRecommendDietListResponses() {
        return recommendDietListResponses;
    }

    public void setRecommendDietListResponses(ArrayList<RecommendDietListResponse> recommendDietListResponses) {
        this.recommendDietListResponses = recommendDietListResponses;
    }

    public RecommendDietListResponse getLastDietListData() {
        return recommendDietListResponses.get(recommendDietListResponses.size() - 1);
    }

}
