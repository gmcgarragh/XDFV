/*******************************************************************************
 *
 *    Copyright (C) 2015-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
 *
 *    This source code is licensed under the GNU General Public License (GPL),
 *    Version 3.  See the file COPYING for more details.
 *
 ******************************************************************************/

#include <gnetcdf.h>

#include <netcdf.h>

#include "xdfv.h"
#include "nctableview.h"


NCTableView::NCTableView(const char *file_name, const char *var_name, QWidget *parent)
     : XDFTableView(parent), file_name(file_name), var_name(var_name)
{
    char temp[NC_MAX_NAME];

    int status;

    int nc_id;
    int var_id;

    int n_dims;
    int dim_ids[NC_MAX_VAR_DIMS];

    int n_atts;

    nc_type xtype;

    status = nc_open(file_name, NC_NOWRITE, &nc_id);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_open(), file_name = %s, %s\n",
                file_name, nc_strerror(status));
        exit(1);
    }

    status = nc_inq_varid(nc_id, var_name, &var_id);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_inq_varid(), varname = %s, %s\n",
                var_name, nc_strerror(status));
        exit(1);
    }

    status = nc_inq_var(nc_id, var_id, temp, &xtype, &n_dims, dim_ids, &n_atts);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_inq_var(), varname = %s, %s\n",
                var_name, nc_strerror(status));
        exit(1);
    }

    status = nc_close(nc_id);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_close(), file_name = %s, %s\n",
                file_name, nc_strerror(status));
        exit(1);
    }

    buildWidget(var_name, n_dims);

    refreshTable();
}



NCTableView::~NCTableView()
{

}



void NCTableView::refreshTable()
{
    int i_row;
    int n_rows;
    int i_col;
    int n_cols;

    char *temp;

    int status;

    int nc_id;
    int var_id;

    int n_dims;
    int dim_ids[NC_MAX_VAR_DIMS];

    int n_atts;

    int data_size;

    void *data;

    void *ptr;

    size_t length;

    size_t dimlen[MAX_VAR_DIMS];

    size_t start[MAX_VAR_DIMS];
    size_t count[MAX_VAR_DIMS];

    nc_type xtype;

    temp = (char *) malloc(LN * sizeof(char));

    status = nc_open(file_name, NC_NOWRITE, &nc_id);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_open(), file_name = %s, %s\n",
                file_name, nc_strerror(status));
        exit(1);
    }

    status = nc_inq_varid(nc_id, var_name, &var_id);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_inq_varid(), varname = %s, %s\n",
                var_name, nc_strerror(status));
        exit(1);
    }

    status = nc_inq_var(nc_id, var_id, temp, &xtype, &n_dims, dim_ids, &n_atts);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_inq_var(), %s, %s\n", var_name, nc_strerror(status));
        exit(1);
    }

    for (int i = 0; i < n_dims; ++i) {
        status = nc_inq_dimlen(nc_id, dim_ids[i], &dimlen[i]);
        if (status != NC_NOERR) {
            fprintf(stderr, "ERROR: nc_inq_dimlen(), %s\n", nc_strerror(status));
            exit(1);
        }
    }

    if (! parseSlice(n_dims, dimlen, &i_row, &n_rows, &i_col, &n_cols, start, count, &length)) {
        data_size = netcdf_data_type_size(xtype);
        if (data_size == 0) {
            fprintf(stderr, "ERROR: netcdf_data_type_size(), var_name = %s\n", var_name);
            exit(1);
        }

        data = malloc(length * data_size);
        if (data == NULL) {
            fprintf(stderr, "ERROR: Memory allocation failed, var_name = %s\n", var_name);
            exit(1);
        }

        status = nc_get_vara(nc_id, var_id, start, count, data);
        if (status != NC_NOERR) {
            fprintf(stderr, "ERROR: nc_get_vara(), %s\n", nc_strerror(status));
            return exit(1);
        }

        configureTable(i_row, n_rows, i_col, n_cols);

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_cols; ++j) {
                ptr = ((char *) data) + (i * n_cols + j) * data_size;
                netcdf_scaler_to_string(xtype, ptr, 0, temp, LN);
                tableWidget()->setItem(i, j, new QTableWidgetItem(temp));
            }
        }

        free(data);
    }

    status = nc_close(nc_id);
    if (status != NC_NOERR) {
        fprintf(stderr, "ERROR: nc_close(), file_name = %s, %s\n",
                file_name, nc_strerror(status));
        exit(1);
    }

    free(temp);
}
