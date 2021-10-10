#include<iostream>
#include <iomanip>
#define M 20
#define N 20
static const double epsilon   = 1.0e-8;
int equal(double a, double b) { return abs(a-b) < epsilon; }
struct Table{
    int m,n;//m rows;n colums
    double matrix[M][N]{};
};
void print_table(Table *tab){
    int counter = 0;
    int i,j;
    std::cout<<"Table"<<++counter<<std::endl;
    std::cout<< "column:"<< std::setw(5) <<"x[i]";
    for(j=1;j<tab->n; j++) {
        std::cout<<std::setw(5)<<"x["<< j<<"]";
    }
    std::cout<<std::setw(5)<<"\n";
    for(i=0;i<tab->m; i++) {
        if (i==0)
            std::cout<<"max:";
        else
            std::cout<<"b["<<i<<"]";
        for(j=0;j<tab->n; j++) {
            std::cout << std::setw(7) << std::setprecision(2) << tab->matrix[i][j];
        }
        std::cout<<std::setw(5)<<"\n";
    }
}
void opor(Table *tab, int row, int column){
    int i,j;
    double opor_solut;
    opor_solut=tab->matrix[row][column];
    if(opor_solut>0)
    for(j=0;j<tab->n;j++)// по столбцам
        tab->matrix[row][j] /= opor_solut;
    else
        std::cout<<"ERROR"<<std::endl;
    for(i=0; i<tab->m; i++) { // по строкам
        double multiplier = tab->matrix[i][column];
        if(i==row) continue;
        for(j=0; j<tab->n; j++) {
            tab->matrix[i][j] -= multiplier * tab->matrix[row][j];//b[i][j]=a[i][j]- a[i][s]*a[r][j]
        }
    }
}
int find_pivot_column(Table *tab) {
    int j, pivot_col = 1;
    double lowest = tab->matrix[0][pivot_col];
    for(j=1; j<tab->n; j++) {
        if (tab->matrix[0][j] < lowest) {
            lowest = tab->matrix[0][j];
            pivot_col = j;
        }
    }
    std::cout<<"Most negative column in row[0] is col "<< pivot_col<<" = "<< lowest<<std::endl;
    if( lowest >= 0 ) {
        return -1; // All positive - optimal.
    }
    return pivot_col;
}
int find_pivot_row(Table *tab, int pivot_col) {
    int i, pivot_row = 0;
    double min_ratio = -1;


    for(i = 1; i < tab->m; i++){//по строкам
        double element = tab->matrix[i][0] / tab->matrix[i][pivot_col];//ищеим разр эл
        if ( (element > 0  && element < min_ratio ) || min_ratio < 0 ) {
            min_ratio = element;
            pivot_row = i;
        }
    }
    if (min_ratio == -1)
        return -1; // Unbounded.
    std::cout<<"Found pivot A["<<pivot_row<<"]"<<"["<<pivot_col<<"]"<< "min positive element= "<<min_ratio<<" in row= "<<pivot_row<<std::endl;
    return pivot_row;
}

void add_slack_variables(Table *tab) {
    int i, j;
    for(i=1; i<tab->m; i++) {
        for(j=1; j<tab->m; j++)
            tab->matrix[i][j + tab->n -1] = (i==j);
    }
    tab->n += tab->m -1;
}
int find_basis_variable(Table *tab, int col) {
    int i, xi=-1;
    for(i=1; i < tab->m; i++) {
        if ( equal(tab->matrix[i][col],1) ) {
            if (xi == -1)
                xi=i;   // found first '1', save this row number.
            else
                return -1; // found second '1', not an identity matrix.

        } else if ( !equal(tab->matrix[i][col],0))  {
            return -1; // not an identity matrix column.
        }
    }
    return xi;
}
void print_optimal_vector(Table *tab, char *message) {
    int j, xi;
    printf("%s at ", message);
    for(j=1;j<tab->n;j++) { // for each column.
        xi = find_basis_variable(tab, j);
        if (xi != -1)
            printf("x%d=%3.2lf, ", j, tab->matrix[xi][0] );
        else
            printf("x%d=0, ", j);
    }
    printf("\n");
}

void simplex(Table *tab) {
    int loop=0;
    add_slack_variables(tab);
    std::cout<<"Padded with slack variables"<<std::endl;
    print_table(tab);
    while( ++loop ) {
        int pivot_col, pivot_row;

        pivot_col = find_pivot_column(tab);
        if( pivot_col < 0 ) {
            std::cout<<"Found optimal value=A[0,0]="<<tab->matrix[0][0]<<"(no negatives in row 0)"<<std::endl;
            print_optimal_vector(tab,"Optimal");
            break;
        }
        std::cout<<"Entering variable x"<<pivot_col<< " to be made basic, so pivot_col="<<pivot_col<<std::endl;


        pivot_row = find_pivot_row(tab, pivot_col);
        if (pivot_row < 0) {
            std::cout<<"unbounded (no pivot_row).\n";
            break;
        }
        std::cout<<"Leaving variable x"<< pivot_row<<"so pivot_row=" <<pivot_row<<std::endl;

        opor(tab, pivot_row, pivot_col);
        std::cout<<"After pivoting"<<std::endl;
        print_table(tab);
        print_optimal_vector(tab,"Solution");
    }
}
int main(){
    Table tab  = { 4, 4, {
            {  0.0 , -3.0 , -3.0 ,-7.0 ,},// f
            { 3.0 ,  1.0 ,  1.0 , 1.0 ,},//1 огранич
            { 5.0 , 1.0 , 4.0 , 0.0 ,},// 2 огранич
            { 7.0 ,  0.0 ,  0.5 , 3.0,},//3 огранич
    }
    };
print_table(&tab);
simplex(&tab);
}
