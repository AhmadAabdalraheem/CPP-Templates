#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    long long x, y;
    long long u, v; // u = x + y,  v = x - y
};

// -------------------------------------------------------------
// 1. أقصى مسافة مانهاتن بين أي نقطتين - O(N)
// -------------------------------------------------------------
long long getMaxManhattanDistance(const vector<Point>& pts) {
    if (pts.size() < 2) return 0;

    long long min_u = 4e18, max_u = -4e18;
    long long min_v = 4e18, max_v = -4e18;

    for (const auto& p : pts) {
        min_u = min(min_u, p.u);
        max_u = max(max_u, p.u);
        min_v = min(min_v, p.v);
        max_v = max(max_v, p.v);
    }

    return max(max_u - min_u, max_v - min_v);
}

// -------------------------------------------------------------
// 2. أقل مسافة مانهاتن بين أي نقطتين متجاورين/مختلفين - O(N log N)
// -------------------------------------------------------------
long long getMinManhattanDistance(vector<Point> pts) {
    int n = pts.size();
    if (n < 2) return 0;

    // بنرتب النقط حسب x الأول
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    long long min_dist = 4e18;

    // فحص المسافات مع الأقرب في x لتقليل المقارنات
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long dx = pts[j].x - pts[i].x;
            // لو الفرق في x لوحده أكبر من أصغر مسافة وصلنا لها، نوقف الـ loop الداخلية
            if (dx >= min_dist) break; 
            
            long long dy = abs(pts[j].y - pts[i].y);
            min_dist = min(min_dist, dx + dy);
        }
    }

    return min_dist;
}

// -------------------------------------------------------------
// دالة تجهيز وتقسيم النقط (Helper Function)
// -------------------------------------------------------------
vector<Point> createPoints(const vector<pair<long long, long long>>& raw_pts) {
    vector<Point> pts;
    for (auto& p : raw_pts) {
        pts.push_back({p.first, p.second, p.first + p.second, p.first - p.second});
    }
    return pts;
}


int main() {
    vector<pair<long long, long long>> input_points = {
        {0, 0}, {10, 0}, {0, 10}, {2, 2}
    };

    // تحويل البيانات للتيمبليت
    vector<Point> pts = createPoints(input_points);

    // 1. أقصى مسافة
    long long max_dist = getMaxManhattanDistance(pts);
    
    // 2. أقل مسافة
    long long min_dist = getMinManhattanDistance(pts);

    cout << "Max Distance: " << max_dist << endl;
    cout << "Min Distance: " << min_dist << endl;

    return 0;
}
