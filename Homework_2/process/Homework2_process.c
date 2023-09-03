#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
# define MAP_ANONYMOUS        0x20                /* Don't use a file.  */
# define MAP_ANON        MAP_ANONYMOUS
#define PRODUCT_SIZE 5
#define CUSTOMER_SIZE 3
struct Product{
    int product_ID;
    double product_Price;
    int product_Quantity;
    
    int init_Quantity;
};

struct Customer{
    int customer_ID;
    double customer_Balance;
    int number_of_ordered_products;
    struct Product ordered_products[PRODUCT_SIZE];
    int order_amount[PRODUCT_SIZE];
    int purchase_amount[PRODUCT_SIZE];
    struct Product purchased_products[PRODUCT_SIZE];
    int purchased_amount[PRODUCT_SIZE];

    double init_Balance;
};

void print_products(struct Product products[PRODUCT_SIZE]){
    printf("Product ID  Quantity    Price\n");
    for(int i=0; i<PRODUCT_SIZE; i++){
        printf("%d           %d           %f\n", products[i].product_ID, products[i].product_Quantity, products[i].product_Price);
    }
}

struct Product* products;
struct Product *products_pointer;

struct Customer *customers;
struct Customer *customers_pointer;

void order_product(struct Customer *customer, struct Product *products_pointer){
    int id = fork();
    if(id!=0)
    wait(NULL);
    if(id==0){
    if((customer->order_amount[0]<=products_pointer[customer->ordered_products[0].product_ID-1].product_Quantity)
        && (customer->customer_Balance >= customer->ordered_products[0].product_Price*customer->order_amount[0])){
        printf("\nCustomer%d:\n", customer->customer_ID);
        printf("Initial Products:\n");
        print_products(products_pointer);
        customer->purchased_products[0].product_ID = customer->ordered_products[0].product_ID;
        customer->purchased_amount[0] = customer->order_amount[0];
        products_pointer[customer->ordered_products[0].product_ID-1].product_Quantity -= customer->order_amount[0];
        customer->customer_Balance -= customer->ordered_products[0].product_Price*customer->order_amount[0];
        printf("\nUpdated Products:\n");
        print_products(products_pointer);
        printf("Bought %d of product %d for $%f\n", customer->order_amount[0], customer->ordered_products[0].product_ID, customer->ordered_products[0].product_Price);
        printf("\n");
        printf("Customer%d(%d,%d) success! Paid $%f for each.\n", customer->customer_ID, customer->ordered_products[0].product_ID, customer->order_amount[0], customer->ordered_products[0].product_Price);
        }
    else{
        if((customer->order_amount[0] > products[0].product_Quantity))
        printf("Customer%d(%d,%d) failiure! Only %d left in stock.\n", customer->customer_ID, customer->ordered_products[0].product_ID, customer->order_amount[0], products_pointer[customer->ordered_products[0].product_ID-1].product_Quantity);
        else if((customer->customer_Balance < customer->ordered_products[0].product_Price*customer->order_amount[0]))
        printf("Customer%d(%d,%d) failiure! Insufficient funds\n", customer->customer_ID, customer->ordered_products[0].product_ID, customer->order_amount[0]);
    }
    exit(0);
    }
}

void order_products(struct Customer *customer, struct Product *products_pointer){
    int id = fork();
    if(id!=0)
    wait(NULL);
    int i = 0;
    if(id==0){
    while(customer->ordered_products[i].product_ID!=0)
    {
    if((customer->order_amount[i]<=products[customer->ordered_products[i].product_ID-1].product_Quantity)
        && (customer->customer_Balance >= customer->ordered_products[i].product_Price*customer->order_amount[i])){
        printf("\nCustomer%d:\n", customer->customer_ID);
        printf("Initial Products:\n");
        print_products(products_pointer);
        customer->purchased_products[i].product_ID = customer->ordered_products[i].product_ID;
        customer->purchased_amount[i] = customer->order_amount[i];
        products[customer->ordered_products[i].product_ID-1].product_Quantity -= customer->order_amount[i];
        customer->customer_Balance -= customer->ordered_products[i].product_Price*customer->order_amount[i];
        printf("\nUpdated Products:\n");
        print_products(products_pointer);
        printf("Bought %d of product %d for $%f\n", customer->order_amount[i], customer->ordered_products[i].product_ID, customer->ordered_products[i].product_Price);
        printf("\n");
        printf("Customer%d(%d,%d) success! Paid $%f for each.\n", customer->customer_ID, customer->ordered_products[i].product_ID, customer->order_amount[i], customer->ordered_products[i].product_Price);
        }
    else{
        if((customer->order_amount[i] > products[customer->ordered_products[i].product_ID-1].product_Quantity))
        printf("Customer%d(%d,%d) failiure! Only %d left in stock.\n", customer->customer_ID, customer->ordered_products[i].product_ID, customer->order_amount[i], products[customer->ordered_products[i].product_ID-1].product_Quantity);
        else if((customer->customer_Balance < customer->ordered_products[0].product_Price*customer->order_amount[i]))
        printf("Customer%d(%d,%d) failiure! Insufficient funds\n", customer->customer_ID, customer->ordered_products[i].product_ID, customer->order_amount[i]);
    }
        i++;
        if(i==PRODUCT_SIZE)
        break;
    }
    exit(0);
    }

}

void print_customer(struct Customer *customer){
    printf("\n");
    printf("Customer%d----------\n", customer->customer_ID);
    printf("initial balance: $%f\n", customer->init_Balance);
    printf("updated balance: $%f\n", customer->customer_Balance);
    printf("Ordered products:\n");
    printf("id      quantity\n");
    int i=0;
    while(customer->ordered_products[i].product_ID!=0){
        printf("%d", customer->ordered_products[i].product_ID);
        printf("        %d\n", customer->order_amount[i]);
        i++;
        if(i==PRODUCT_SIZE)
        break;
    }

    printf("\n");
    printf("Purchased products:\n");
    printf("id      quantity\n");
    i=0;
    while(i!=PRODUCT_SIZE){
        if(customer->purchased_products[i].product_ID!=0){
        printf("%d", customer->purchased_products[i].product_ID);
        printf("        %d\n", customer->order_amount[i]);
        }
        i++;

    }
}


int main(){
    srand(time(NULL));
    products_pointer = mmap(NULL, sizeof(struct Product)*PRODUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    customers_pointer= mmap(NULL, sizeof(struct Customer)*PRODUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    products = products_pointer;
    customers = customers_pointer;
    for(int i=0; i<PRODUCT_SIZE; i++){
        products[i].product_ID=i+1;
        products[i].product_Price=(double)((double)rand()/RAND_MAX)*199+1;
        products[i].product_Quantity=rand()%10+1;
        products[i].init_Quantity = products[i].product_Quantity;
    }
    
    for(int i=0; i<CUSTOMER_SIZE; i++){
        customers[i].customer_ID=i+1;
        customers[i].customer_Balance=(double)((double)rand()/RAND_MAX)*200;
        customers[i].number_of_ordered_products = rand()%PRODUCT_SIZE+1;

        for(int j=0; j<PRODUCT_SIZE; j++){
            customers[i].ordered_products[j].product_ID=0;
            customers[i].purchased_products[j].product_ID=0;
        }

        for(int j=0; j<customers[i].number_of_ordered_products; j++){
            customers[i].ordered_products[j] = products[rand()%PRODUCT_SIZE];
            customers[i].order_amount[j] = rand()%5+1;
            customers[i].purchased_amount[j] = 0;
            for(int k=0; k<j; k++){
                if(customers[i].ordered_products[j].product_ID == customers[i].ordered_products[k].product_ID){
                    customers[i].ordered_products[j] = products[rand()%PRODUCT_SIZE];
                    k=-1;
                }
            }
        }
        customers[i].init_Balance = customers[i].customer_Balance;
    }
    printf("\n");

    for(int i=0; i<CUSTOMER_SIZE; i++){
        if(customers[i].number_of_ordered_products == 1){
            order_product(&customers[i], products);
        }

        else{
            order_products(&customers[i], products);
        }
    }

    for(int i=0; i<CUSTOMER_SIZE; i++){
        print_customer(&customers[i]);
    }
    munmap(products_pointer, sizeof(struct Product)*PRODUCT_SIZE);
    munmap(customers_pointer, sizeof(struct Customer)*PRODUCT_SIZE);

    return 0;
    }