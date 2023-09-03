#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#define PRODUCT_SIZE 5
#define CUSTOMER_SIZE 3
pthread_mutex_t productMutex;
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
struct Product products[PRODUCT_SIZE];

void print_products(struct Product products[PRODUCT_SIZE]){
    printf("Product ID  Quantity    Price\n");
    for(int i=0; i<PRODUCT_SIZE; i++){
        printf("%d           %d           %f\n", products[i].product_ID, products[i].product_Quantity, products[i].product_Price);
    }
}

void *order_product(void *args){
    struct Customer *customer = (struct Customer*) args;
    pthread_mutex_lock(&productMutex);
    if((customer->order_amount[0]<=products[customer->ordered_products[0].product_ID-1].product_Quantity)
        && (customer->customer_Balance >= customer->ordered_products[0].product_Price*customer->order_amount[0])){
        printf("\nCustomer%d:\n", customer->customer_ID);
        printf("Initial Products:\n");
        print_products(products);
        customer->purchased_products[0].product_ID = customer->ordered_products[0].product_ID;
        customer->purchased_amount[0] = customer->order_amount[0];
        products[customer->ordered_products[0].product_ID-1].product_Quantity -= customer->order_amount[0];
        customer->customer_Balance -= customer->ordered_products[0].product_Price*customer->order_amount[0];
        printf("\nUpdated Products:\n");
        print_products(products);
        printf("Bought %d of product %d for $%f\n", customer->order_amount[0], customer->ordered_products[0].product_ID, customer->ordered_products[0].product_Price);
        printf("\n");
        printf("Customer%d(%d,%d) success! Paid $%f for each.\n", customer->customer_ID, customer->ordered_products[0].product_ID, customer->order_amount[0], customer->ordered_products[0].product_Price);
        }
    else{
        if((customer->order_amount[0] > products[0].product_Quantity))
        printf("Customer%d(%d,%d) failiure! Only %d left in stock.\n", customer->customer_ID, customer->ordered_products[0].product_ID, customer->order_amount[0], products[customer->ordered_products[0].product_ID-1].product_Quantity);
        else if((customer->customer_Balance < customer->ordered_products[0].product_Price*customer->order_amount[0]))
        printf("Customer%d(%d,%d) failiure! Insufficient funds\n", customer->customer_ID, customer->ordered_products[0].product_ID, customer->order_amount[0]);
    }
    pthread_mutex_unlock(&productMutex);
}

void *order_products(void *args){
    struct Customer *customer = (struct Customer*) args;
    int i = 0;
    pthread_mutex_lock(&productMutex);
    while(customer->ordered_products[i].product_ID!=0)
    {
    if((customer->order_amount[i]<=products[customer->ordered_products[i].product_ID-1].product_Quantity)
        && (customer->customer_Balance >= customer->ordered_products[i].product_Price*customer->order_amount[i])){
        printf("\nCustomer%d:\n", customer->customer_ID);
        printf("Initial Products:\n");
        print_products(products);
        customer->purchased_products[i].product_ID = customer->ordered_products[i].product_ID;
        customer->purchased_amount[i] = customer->order_amount[i];
        products[customer->ordered_products[i].product_ID-1].product_Quantity -= customer->order_amount[i];
        customer->customer_Balance -= customer->ordered_products[i].product_Price*customer->order_amount[i];
        printf("\nUpdated Products:\n");
        print_products(products);
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
    pthread_mutex_unlock(&productMutex);
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
    for(int i=0; i<PRODUCT_SIZE; i++){
        products[i].product_ID=i+1;
        products[i].product_Price=(double)((double)rand()/RAND_MAX)*199+1;
        products[i].product_Quantity=rand()%10+1;
        products[i].init_Quantity = products[i].product_Quantity;
    }

    pthread_mutex_init(&productMutex, NULL);
    pthread_t customer_threads[CUSTOMER_SIZE];

    struct Customer customers[CUSTOMER_SIZE];
    for(int i=0; i<CUSTOMER_SIZE; i++){
        customers[i].customer_ID=i+1;
        customers[i].customer_Balance=(double)((double)rand()/RAND_MAX)*200;
        customers[i].number_of_ordered_products = rand()%PRODUCT_SIZE+1;

        for(int j=0; j<PRODUCT_SIZE; j++){ //new, to deal with 0 initialization
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
            pthread_create(&customer_threads[i], NULL, &order_product, (void*) &customers[i]);
        }

        else{
            pthread_create(&customer_threads[i], NULL, &order_products, (void*) &customers[i]);
        }
    }

    for(int i=0; i<CUSTOMER_SIZE; i++){
        pthread_join(customer_threads[i], NULL);
    }
    for(int i=0; i<CUSTOMER_SIZE; i++){
        print_customer(&customers[i]);
    }
    pthread_mutex_destroy(&productMutex);
    return 0;
    }