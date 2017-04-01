#include <vector>
#include <string>
#include <math.h>
#include <cmath>

#include "config.hpp"
#include "World.h"
#include "Entry.h"
#include "utils.h"
#include "LineRecord.h"

#include "xlsxwriter.h"

using namespace std;

void outputFile(vector<Entry*> entries, World *w) {
    bool compress = w->compress;

    lxw_workbook *wb = workbook_new(string(w->outputFile+".xlsx").c_str());
    lxw_worksheet *ws = workbook_add_worksheet(wb, "result");

    lxw_format *float02 = workbook_add_format(wb);
    lxw_format *float03 = workbook_add_format(wb);
    lxw_format *float01 = workbook_add_format(wb);
    lxw_format *floatSUM = workbook_add_format(wb);
    format_set_num_format(float01, "0.0");
    format_set_num_format(float02, "0.00");
    format_set_num_format(float03, "0.000");
    format_set_bold(floatSUM);

    size_t ci = 0;

    if(compress) {
        worksheet_write_string(ws, 0, ci++, "TIME", NULL);
    } else {
        worksheet_write_string(ws, 0, ci++, "TIME", NULL);

        for(size_t i = 0; i < w->systemsCount; ++i) {
            worksheet_write_string(ws, 0, ci++, string("U" + i).c_str(), NULL);
        }
    }

    double unit = 0;

    if(!compress) {
        for(size_t i = 0; i < w->lines.size(); ++i) {
            LineRecord *dp = entries.at(0)->dp[i];

            worksheet_write_string(ws, 0, ci++, string(dp->label+"("+floatFormat(dp->ll, 0)+"km,AC-"+floatFormat((double) dp->wa, 0)+")").c_str(), NULL);
            worksheet_write_string(ws, 0, ci++, "P", NULL);
            worksheet_write_string(ws, 0, ci++, "Q", NULL);
            worksheet_write_string(ws, 0, ci++, "dP", NULL);
            worksheet_write_string(ws, 0, ci++, "dQ", NULL);
            worksheet_write_string(ws, 0, ci++, "dP_corona", NULL);
        }
    }

    if(compress) {
        worksheet_write_string(ws, 0, ci++, "WEATHER", NULL);
        worksheet_write_string(ws, 0, ci++, "dP", NULL);
        worksheet_write_string(ws, 0, ci++, "dQ", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_corona", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_corona_kW_km", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_loss%", NULL);
        worksheet_write_string(ws, 0, ci++, "dQ_loss%", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_corona_loss%", NULL);
    } else {
        worksheet_write_string(ws, 0, ci++, "TEMP", NULL);
        worksheet_write_string(ws, 0, ci++, "HUMIDITY%", NULL);
        worksheet_write_string(ws, 0, ci++, "PRESSURE", NULL);
        worksheet_write_string(ws, 0, ci++, "WEATHER", NULL);
        worksheet_write_string(ws, 0, ci++, "COMBINED_ACTIVE", NULL);
        worksheet_write_string(ws, 0, ci++, "COMBINED_REACTIVE", NULL);
        worksheet_write_string(ws, 0, ci++, "dP", NULL);
        worksheet_write_string(ws, 0, ci++, "dQ", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_corona", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_corona_kW_km", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_loss%", NULL);
        worksheet_write_string(ws, 0, ci++, "dQ_loss%", NULL);
        worksheet_write_string(ws, 0, ci++, "dP_corona_loss%", NULL);
    }

    double *lineP = new double[w->lines.size()];
    double *lineQ = new double[w->lines.size()];
    double *linePQ = new double[w->lines.size()];

    double lossP = 0,
           lossQ = 0,
           lossPQ = 0;

    size_t i;
    for(i = 0; i < entries.size(); ++i) {
        size_t row = i + 1;

        ci = 0;

        Entry *e = entries.at(i);

        worksheet_write_number(ws, row, ci++, (double) i, NULL);

        active_power_t intervalLossPowerCombined = 0;
        active_power_t intervalLossPowerCombinedUnit = 0;

        active_power_t   pLossesCombined = 0;
        reactive_power_t qLossesCombined = 0;

        if(!compress) {
            for(size_t i = 0; i < w->systemsCount; ++i) {
                worksheet_write_number(ws, row, ci++, e->S[i], NULL);
            }
        }

        for(size_t j = 0; j < w->lines.size(); ++j) {
            LineRecord *dp = e->dp[j];

            if(!compress) {
                worksheet_write_string(ws, row, ci++, "", NULL);
                worksheet_write_number(ws, row, ci++, dp->P, float01);
                worksheet_write_number(ws, row, ci++, dp->Q, float01);
                worksheet_write_number(ws, row, ci++, dp->dP*dp->ll, float03);
                worksheet_write_number(ws, row, ci++, dp->dQ*dp->ll, float03);
                worksheet_write_number(ws, row, ci++, dp->ilp*dp->ll, float03);

                lineP[j] += dp->dP*dp->ll;
                lineQ[j] += dp->dQ*dp->ll;
                linePQ[j] += dp->ilp*dp->ll;
            }

            intervalLossPowerCombinedUnit += dp->ilp * 1000.0;
            intervalLossPowerCombined += dp->ilp * dp->ll;
            qLossesCombined += dp->dQ * dp->ll;
            pLossesCombined += dp->dP * dp->ll;
        }

        lossP += pLossesCombined;
        lossQ += qLossesCombined;
        lossPQ += intervalLossPowerCombined;

        if(!compress) worksheet_write_number(ws, row, ci++, e->temperature, NULL);
        if(!compress) worksheet_write_number(ws, row, ci++, e->humidity*100, NULL);
        if(!compress) worksheet_write_number(ws, row, ci++, e->relativePressure, NULL);

        worksheet_write_string(ws, row, ci++, weatherTypeToString(e->weather_type).c_str(), NULL);

        if(!compress) worksheet_write_number(ws, row, ci++, e->combinedTransmissionP, float03);
        if(!compress) worksheet_write_number(ws, row, ci++, e->combinedTransmissionQ, float03);

        worksheet_write_number(ws, row, ci++, pLossesCombined, float03);
        worksheet_write_number(ws, row, ci++, qLossesCombined, float03);
        worksheet_write_number(ws, row, ci++, intervalLossPowerCombined, float03);
        worksheet_write_number(ws, row, ci++, intervalLossPowerCombinedUnit / w->lines.size(), float03);

        if(w->useFullLossPower) {
            double flp = pow(pow(pLossesCombined + intervalLossPowerCombined, 2) + pow(qLossesCombined, 2), 0.5);

            worksheet_write_number(ws, row, ci++, flp > 0.1 ? pLossesCombined / flp * 100 : 0, float02);
            worksheet_write_number(ws, row, ci++, flp > 0.1 ? qLossesCombined / flp * 100 : 0, float02);
            worksheet_write_number(ws, row, ci++, flp > 0.1 ? intervalLossPowerCombined / flp * 100 : 0, float02);
        } else if(w->useLossesActive) {
            worksheet_write_number(ws, row, ci++, pLossesCombined > 0.1 ? pLossesCombined / (intervalLossPowerCombined + pLossesCombined) * 100 : 0, float02);
            worksheet_write_number(ws, row, ci++, pLossesCombined > 0.1 ? qLossesCombined / (intervalLossPowerCombined + pLossesCombined) * 100 : 0, float02);
            worksheet_write_number(ws, row, ci++, pLossesCombined > 0.1 ? intervalLossPowerCombined / (intervalLossPowerCombined + pLossesCombined) * 100 : 0, float02);
        } else {
            worksheet_write_number(ws, row, ci++, pLossesCombined / e->combinedTransmissionP * 100, float02);
            worksheet_write_number(ws, row, ci++, qLossesCombined / e->combinedTransmissionQ * 100, float02);
            worksheet_write_number(ws, row, ci++, pLossesCombined > 0.1 ? intervalLossPowerCombined / (intervalLossPowerCombined + pLossesCombined + qLossesCombined) * 100 : 0, float02);
        }

        unit += (intervalLossPowerCombinedUnit / w->lines.size());
    }

    if(compress) {

    } else {
        size_t lr = i + 1;

        for(size_t k = 0; k < w->lines.size(); ++k) {
            size_t index = 1 + w->systemsCount + k*6;

            worksheet_write_number(ws, lr, index + 3, lineP[k], floatSUM);
            worksheet_write_number(ws, lr, index + 4, lineQ[k], floatSUM);
            worksheet_write_number(ws, lr, index + 5, linePQ[k], floatSUM);
        }

        size_t index = 1 + w->systemsCount + 6*w->lines.size();

            worksheet_write_number(ws, lr, index + 6, lossP, floatSUM);
            worksheet_write_number(ws, lr, index + 7, lossQ, floatSUM);
            worksheet_write_number(ws, lr, index + 8, lossPQ, floatSUM);
    }

    if(w->verbose) printf("UNIT: %.3f\n", unit / 8760);

    workbook_close(wb);
}
