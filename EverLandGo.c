#include <stdio.h>
#include <time.h> 
#include <string.h>

//함수들은 독립적으로

    // 이용금액
    const int BABY_PRICE =0, //36개월 미만
            ADULT_TEEN_A_PRICE = 62000, ADULT_TEEN_B_PRICE = 52000,
            ADULT_TEEN_C_PRICE = 46000, ADULT_TEEN_D_PRICE = 68000,
		    CHILD_OLD_A_PRICE = 52000, CHILD_OLD_B_PRICE = 42000,
		    CHILD_OLD_C_PRICE = 36000, CHILD_OLD_D_PRICE = 58000; 
    const int PRICE[]={0, 62000, 52000, 46000, 68000, 52000, 42000, 36000, 58000};
    const char *NAME[]={"BABY","ADULT_TEEN_A","ADULT_TEEN_B"
                        ,"ADULT_TEEN_C","ADULT_TEEN_D"
                        ,"CHILD_OLD_A","CHILD_OLD_B"
                        ,"CHILD_OLD_C","CHILD_OLD_C"};

    // 나이에 따른 범위
    const int MIN_BABY = 1, MIN_CHILD = 3, MIN_TEEN = 13, MIN_ADULT = 19,
		     MAX_CHILD = 12, MAX_TEEN = 18, MAX_ADULT = 64;

   
    // 나이에 따른 그룹
    const int BABY = 1, CHILD = 2, ADULT_TEEN = 3, OLD = 4;

    // 할인율
    const float DISABLE_DISCOUNT_RATE = 0.6, MERIT_DISCOUNT_RATE = 0.5,
			MULTICHILD_DISCOUNT_RATE = 0.8, REGNANT_DISCOUNT_RATE = 0.85;

    // 최대 주문량
    const int MAX_COUNT = 10, MIN_COUNT = 1;


/// @brief 
struct product {
        int totalSelect;  // 티켓 종류 선택
        int totalQuantity;
        int totalSum;
        char totalDiscout[50]; // 티켓 우대 문자열
        char totalName[50];  // 티켓 종류 문자열      
    } ;



int inputTicketSelect();
int inputCustomerIDNumbe();
int calcAgeGroup(int age, int select);
int inputOrderCount();
//int inputDiscountSelect(int select, int quantity);
int inputDiscountSelect(int select, int quantity, int finalCount, struct product *products, struct product p);
int inputCheckRestart(int EachSum);

int reStrart();

int printTickets(struct product *products, int finalCount);
//int qClose();

// int calcPriceResult(int calcPrice, int orderCount){

// }



int main(){
    
while(1){
    // 구조체 변수 선언
    int select=0, age=0,quantity=0;
    int  discount, close, sum;
    char discountStat[20];
    int TotalTotalSum=0;
    struct product products[100];
    
     int count=0;
     int finalCount=0;
    

   
    while(1) {   
        select = inputTicketSelect();  // 티켓 종류 선택              
        age = inputCustomerIDNumbe();  // 나이 확인
        printf("만나이 : %d\n",age);
        select = calcAgeGroup(age, select); // 나이에 따른 우대 적용 확인

        products[finalCount].totalSelect = select; // 구조체에 티켓 종류 넣어주기
        products[finalCount].totalName[finalCount] = '\0';   // 티켓종류 이름에 공백 넣어주기
        strcpy(products[finalCount].totalName, NAME[select]);  // 문자열 넣어주기 
 
        quantity = inputOrderCount(); // input 수량 함수 출력
        count=count+quantity; // 총 수량 누적
        products[finalCount].totalQuantity= quantity;

        int EachSum= inputDiscountSelect(select, quantity,finalCount,products,products[finalCount]); // 1회 돌렸을 때의 우대율을 고려한 금액      
        products[finalCount].totalSum= EachSum;
        finalCount++;

        close = inputCheckRestart(EachSum);
        getchar();   // 버퍼 비우기

        if(close==2){
            break;
        }   
    } 

    int i=0; // for문을 위한 index 변수
    TotalTotalSum=  printTickets(products, finalCount); 
        close = reStrart();
        if(close==2){
         break;
        }     
     }
    return 0;
}



int inputTicketSelect(){
    
    int ticket;
    printf("1. A\t2. B\t3. C\t4. D\n");
    scanf("%d",&ticket);
    return ticket;
}

int inputCustomerIDNumbe(){
    // 주민번호 입력 하면 만나이 계산

    printf("주민번호를 입력하세요\t(입력형식 : yymmdd123456)\n");
    
    char id[15];
    scanf("%s",id);
 	int year, month, day;
	int current_year, current_month, current_day;
	int age=0, gender=0;	
	
	year = (id[0]-'0')*10+(id[1]-'0');
	month=(id[2]-'0')*10+(id[3]-'0');
	day=(id[4]-'0')*10+(id[5]-'0');
    gender=((int)id[6]);
    	
	if (gender=='3' || gender=='4'){
		year+=2000;
	} else{
		year+=1900;
	}		

	time_t current_time = time(NULL);	
	struct tm *tm= localtime(&current_time);
	current_year = tm->tm_year + 1900;
	current_month = tm->tm_mon+1;
	current_day=tm->tm_mday;
	
	age=current_year-year;
	 if((current_month<month) || ((current_month==month) && (current_day<day))){  
	 	age--;
	 }	  
	return age;
} 

int calcAgeGroup(int age, int select){
    if(age<MIN_CHILD){
        return select=0;
    }else if(age<MIN_TEEN||age>MAX_ADULT){
        return select=select+4;
    }else{
        return select;
     }   
}

int inputOrderCount(){
    int quantity=0;
    printf("몇개를 주문하시겠습니까? (최대 10개)\n");
        scanf("%d",&quantity);
        if ( quantity>MAX_COUNT || quantity<MIN_COUNT ){
            printf("최소 1개 이상 최대 10개 이하 주문 가능합니다.\n");
            return inputOrderCount();
        }
    return quantity;
}

int inputDiscountSelect(int select, int quantity,int finalCount,struct product *products,struct product p){
        int discount=0;
        printf("우대사항을 선택하세요.\n");
        printf("1. 없음 (나이 우대는 자동처리)\n2. 장애인\n3. 국가유공자\n4. 다자녀\n5. 임신부\n");

        scanf("%d",&discount);

        int EachSum=0;                
        switch (discount)
        {
        case 1:            
            EachSum = PRICE[select]*quantity;
            strcpy(products[finalCount].totalDiscout,"*우대적용 없음");
            return EachSum;
            break;
        case 2:
            EachSum = DISABLE_DISCOUNT_RATE*PRICE[select]*quantity;
            strcpy(products[finalCount].totalDiscout,"*장애인 우대적용");
            return EachSum;
            break;
        case 3:
            EachSum = MERIT_DISCOUNT_RATE*PRICE[select]*quantity;
            strcpy(products[finalCount].totalDiscout,"*국가유공자 우대적용");
            return EachSum;
            break;
        case 4:
            EachSum = MULTICHILD_DISCOUNT_RATE*PRICE[select]*quantity;
            strcpy(products[finalCount].totalDiscout,"*다자녀 우대적용");
            return EachSum;
            break;
        case 5:
            EachSum = REGNANT_DISCOUNT_RATE*PRICE[select]*quantity;
            strcpy(products[finalCount].totalDiscout,"*임산부 우대적용");
            return EachSum;
            break;       
         }         
}

int inputCheckRestart(int EachSum){
    printf("가격은 %d 원 입니다.\n",EachSum);
    printf("감사합니다.\n");

    int close=1;
    printf("계속 발권 하시겠습니까?\n");
    printf("1. 티켓 발권\n2. 종료\n");
    scanf("%d",&close);

    return close;
}

int reStrart(){
    int close=1;
    printf("계속 진행(1: 새로운 주문, 2: 프로그램 종료) : \n");  
    scanf("%d",&close);
    return close;
}

// 하단부 출력
int printTickets(struct product *products, int finalCount) {
    int TotalTotalSum = 0;
    printf("\n티켓 발권을 종료합니다. 감사합니다.\n\n");
    printf("==================== 에버랜드 ====================\n");
    for(int i=0; i<finalCount; i++){
        printf("%-15s * %3d  %10d원   %-10s \n",
        products[i].totalName,products[i].totalQuantity,products[i].totalSum,products[i].totalDiscout);
    }

    for(int i=0; i<finalCount; i++){
        TotalTotalSum=TotalTotalSum+products[i].totalSum;
    }

    printf("입장료 총액은 %d원 입니다.\n",TotalTotalSum);
    printf("==================================================\n");
    return TotalTotalSum;
}