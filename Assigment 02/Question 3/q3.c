#include <stdio.h>

void getReadings(int s, int h, int temp[50][50]) {
    int i, j;
    for (i = 0; i < s; i++) {
        printf("Sensor %d: ", i + 1);
        for (j = 0; j < h; j++) {
            scanf("%d", &temp[i][j]);
        }
    }
}

int findHighestAvg(int s, int h, int temp[50][50], float *highest_avg) {
    int i, j;
    int best_sensor = 1;
    *highest_avg = -999.0;

    for (i = 0; i < s; i++) {
        int sum = 0;
        for (j = 0; j < h; j++) {
            sum += temp[i][j];
        }
        float avg = (float)sum / h;

        if (i == 0 || avg > *highest_avg) {
            *highest_avg = avg;
            best_sensor = i + 1;
        }
    }
    return best_sensor;
}

int main() {
    int s, h, i, j;
    int temp[50][50];

    printf("Enter number of sensors: ");
    scanf("%d", &s);
    printf("Enter number of hourly readings: ");
    scanf("%d", &h);

    printf("Enter temperature readings:\n");
    getReadings(s, h, temp);
    printf("\n");

    for (i = 0; i < s; i++) {
        int total = 0;
        int max = temp[i][0];
        int min = temp[i][0];
        int critical = 0;
        int warning = 0;
        int streak = 0;
        int max_streak = 0;

        for (j = 0; j < h; j++) {
            int val = temp[i][j];
            total += val;

            if (val > max) max = val;
            if (val < min) min = val;

            if (val >= 40) {
                critical++;
            } else if (val >= 35 && val <= 39) {
                warning++;
            }

            if (val >= 35) {
                streak++;
                if (streak > max_streak) {
                    max_streak = streak;
                }
            } else {
                streak = 0;
            }
        }

        float avg = (float)total / h;

        printf("Sensor %d: Total=%d   Avg=%.2f   Max=%d   Min=%d\n", i + 1, total, avg, max, min);
        printf("          Critical=%d   Warning=%d   Longest hot streak=%d\n", critical, warning, max_streak);
    }

    float top_avg = 0.0;
    int top_sensor = findHighestAvg(s, h, temp, &top_avg);

    printf("\nHighest average: Sensor %d (%.2f)\n", top_sensor, top_avg);

    return 0;
}
