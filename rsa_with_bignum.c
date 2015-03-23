//   http://people.eku.edu/styere/Encrypt/RSAdemo.html
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define MAXLENGTH 1024
#define CUTOFF 4
struct bignum{
                int num[MAXLENGTH+1];
                int sign;
                int length;
};
struct bignum zero,one,ten;
void bignum_print(struct bignum x)
{
    int i;
    if(x.sign&1)
        printf("-");
     for(i=0;i<x.length;i++)
        printf("%d",x.num[i]);
        printf("\n");
        return ;
}
int bignum_even(struct bignum x)
{
    if(x.num[x.length-1]%2==0)
        return 0;
    else
        return 1;
}
int bignum_compare(struct bignum x,struct bignum y)
{
    if(x.sign==1 && y.sign==0)
        return -1;
    else if(x.sign==0 && y.sign==1)
        return 1;
    else{
            if(x.sign ==0)
            {
                if(x.length>y.length)
                    return 1;
                else if (x.length<y.length)
                    return -1;
                else{
                    int i,count=0;
                    if(x.num[0]>y.num[0])
                        return 1;
                    else if(x.num[0]<y.num[0])
                        return -1;
                    for(i=1;i<x.length;i++)
                        if(x.num[i]>y.num[i])
                    {
                        count++;
                        return 1;
                    }
                    else if(x.num[i]<y.num[i])
                    {
                        if(count>0)
                            return -1;
                        else return 1;
                    }
                    else{count++;}
                    if(count==(x.length-1))
                        return 0;
                }

            }
            else
                {
                if(x.length>y.length)
                    return -1;
                else if (x.length<y.length)
                    return 1;
                else{
                    int i,count=0;
                    if(x.num[0]>y.num[0])
                        return -1;
                    else if(x.num[0]<y.num[0])
                        return 1;
                    for(i=1;i<x.length;i++)
                        if(x.num[i]>y.num[i])
                    {
                        count++;
                        return -1;
                    }
                    else if(x.num[i]<y.num[i])
                    {
                        if(count>0)
                            return 1;
                        else return -1;
                    }
                    else{count++;}
                    if(count==(x.length-1))
                        return 0;
                }

            }

    }
}

int bignum_magnitude(struct bignum x,struct bignum y)
{
       if(x.length>y.length)
                    return 1;
                else if (x.length<y.length)
                    return -1;
                else{
                    int i,count=0;
                    if(x.num[0]>y.num[0])
                        return 1;
                    else if(x.num[0]<y.num[0])
                        return -1;
                    for(i=1;i<x.length;i++)
                        if(x.num[i]>y.num[i])
                    {
                        return 1;
                    }
                    else if(x.num[i]<y.num[i])
                    {
                      //  if(count>0)
                            return -1;
                    }
                    else{count++;}
                    if(count==(x.length-1))
                        return 0;
                }
}
int find1(int a, int b)
{
    int t = 1;
    while(t <= a || t <=b){
        t=t<<1;
    }
    return t;
}
void initialize(int xx[], int yy[], int xLen, int yLen, int tLen)
{
    int i;
    for(i=xLen; i<tLen; i++) xx[i] = 0;
    for(i=yLen; i<tLen; i++) yy[i] = 0;
}
//standard multiplication
void normalMultiplication(int *xx, int *yy, int *rr, int tLen)
{
    int i;
    int j;
    for(i=0; i<2*tLen; i++)  rr[i] = 0;
    for(i=0; i<tLen; i++){
        for(j=0; j<tLen; j++){
            rr[i+j] += xx[i]*yy[j];
        }
    }
}

void karatsuba(int xx[], int yy[], int rr[], int tLen)
{
    int *a = &xx[tLen/2];
    int *b = &xx[0];
    int *c = &yy[tLen/2];
    int *d = &yy[0];

    //since only 2d space is required for result
    //hence we 'll use remaining space
    int *wx = &rr[tLen*5]; //sum of xx's halves
    int *wy = &rr[tLen*5 + tLen/2]; //sum of yy's halves

    int *V = &rr[tLen*0];  //location of b*d
    int *U = &rr[tLen*1];  //location of a*c
    int *W = &rr[tLen*2];  //location of (a+b)*(c+d)

    //for small value of tLen normal multiplication is faster
    if(tLen <= CUTOFF){
        normalMultiplication(xx, yy, rr, tLen);
        return;
    }
    int i;
    //compute wx and wy
    for(i=0; i<tLen/2; i++){
        wx[i] = a[i] + b[i];
        wy[i] = c[i] + d[i];
    }

    //divide
    karatsuba(b, d, V, tLen/2);
    karatsuba(a, c, U, tLen/2);
    karatsuba(wx, wy, W, tLen/2);

    //conquer and combine
    for(i=0; i<tLen; i++)  W[i]=W[i]-U[i]-V[i];
    for(i=0; i<tLen; i++)  rr[i+tLen/2] += W[i];

}

//to make sure that all numbers are b/w 0-9
void fixCarry(int rr[], int tLen)
{
    int i;
    int carry = 0;
    for(i=0; i<tLen; i++){
        rr[i]+=carry;
        if(rr[i] < 0){
            carry = ( -(rr[i]+1)/10 +1 );
        }else{
            carry = rr[i]/10;
        }
        rr[i] = rr[i]- carry*10;
    }
    assert(carry == 0);  //check overflow
}

void bignum_multiply(struct bignum *res,struct bignum p, struct bignum q)
{
    (*res).sign=(p.sign+q.sign)%2;
    int xx[MAXLENGTH],yy[MAXLENGTH],zz[6*MAXLENGTH];
    int xLen=p.length,yLen=q.length,tLen;
    int i;
    for(i=0;i<xLen;i++)
        xx[xLen-i-1]=p.num[i];
    for(i=0;i<yLen;i++)
        yy[yLen-1-i]=q.num[i];
    tLen = find1(xLen, yLen);
    initialize(xx, yy, xLen, yLen, tLen);
    karatsuba(xx, yy, zz, tLen);

//since we didn't checked in karatsuba that digits are b/w 0-9
    fixCarry(zz, 2 * tLen);
    int count=0;
    int j;
    int n=8*tLen/5 + tLen/8;
    for( j = n; j>0; j--)   if(zz[j] != 0)  break;
    for( ; j>=0; j--)    (*res).num[count++]=zz[j];
    (*res).length=count;
    return;
}

void bignum_subtract(struct bignum *res,struct bignum x,struct bignum y)
{
   struct bignum temp;
 //  memset(temp.num,-1,sizeof(temp.num));
   int i,j,k;
   y.sign=(y.sign+1)%2;
   /*
   for(i=0;i<p.length;i++)
      x.num[i]=p.num[i];
      x.length=p.length;
      x.sign=p.sign;
   for(i=0;i<q.length;i++)
      y.num[i]=q.num[i];
      y.length=q.length;
      y.sign=q.sign;
      */
   if(x.length>y.length)
   {
       k=x.length-y.length;
       for(j=x.length;j>=k;j--)
          y.num[j]=y.num[j-k];
       y.length=x.length;
       for(j=0;j<k;j++)
          y.num[j]=0;
   }
   else if(y.length>x.length)
   {
       k=y.length-x.length;
       for(j=y.length;j>=k;j--)
          x.num[j]=x.num[j-k];
       x.length=y.length;
       for(j=0;j<k;j++)
          x.num[j]=0;
   }
  // bignum_print(x);bignum_print(y);
   if(x.sign==y.sign)
   {
       int  e=0;                                       // carry
         for(i=x.length;i>=0;i--)
           {
                temp.num[i+1]=x.num[i]+y.num[i]+e;
                if(temp.num[i+1]>=10)
                {
                     e=temp.num[i+1]/10;
                     temp.num[i+1]%=10;
                }
                else e=0;
           }

         temp.num[0]=e;
         temp.sign=x.sign;
         temp.length=x.length+1;
   }
   else if(x.sign==1)
   {
       int r;
       r=bignum_magnitude(x,y);
       if(r==0)
         {
             temp.sign=0;
             temp.length=1;
             temp.num[0]=0;
         }
       else if(r==1)
       {
           for(i=x.length-1;i>=0;i--)
             {
                if(x.num[i]>=y.num[i])
                   temp.num[i]=x.num[i]-y.num[i];
                else {
                        if(i>=1)
                        x.num[i-1]--;
                        x.num[i]+=10;
                        temp.num[i]=x.num[i]-y.num[i];
                     }
            }
            temp.length=x.length;
            temp.sign=x.sign;
       }
       else{
             for(i=y.length-1;i>=0;i--)
                {
                 if(y.num[i]>=x.num[i])
                   temp.num[i]=y.num[i]-x.num[i];
                else {
                        if(i>=1)
                        y.num[i-1]--;
                        y.num[i]+=10;
                        temp.num[i]=y.num[i]-x.num[i];
                     }
                 }
             temp.length=y.length;
             temp.sign=y.sign;

       }
   }

else
   {
       int r;
       r=bignum_magnitude(x,y);
       if(r==0)
         {
             temp.sign=0;
             temp.length=1;
             temp.num[0]=0;
         }
       else if(r==1)
       {
           for(i=x.length-1;i>=0;i--)
             {
                if(x.num[i]>=y.num[i])
                   temp.num[i]=x.num[i]-y.num[i];
                else {
                        if(i>=1)
                        x.num[i-1]--;
                        x.num[i]+=10;
                        temp.num[i]=x.num[i]-y.num[i];
                     }
            }
            temp.length=x.length;
            temp.sign=x.sign;
       }
       else{
             for(i=y.length-1;i>=0;i--)
                {
                 if(y.num[i]>=x.num[i])
                   temp.num[i]=y.num[i]-x.num[i];
                else {
                        if(i>=1)
                        y.num[i-1]--;
                        y.num[i]+=10;
                        temp.num[i]=y.num[i]-x.num[i];
                     }
                 }
             temp.length=y.length;
             temp.sign=y.sign;

       }
   }

if(temp.length==1)
{
    (*res).sign=temp.sign;
    (*res).length=temp.length;
    (*res).num[0]=temp.num[0];
    return ;
}
int count=0,clr=0;
(*res).sign=temp.sign;
for(i=0;i<temp.length;i++)
    {
        if(temp.num[i]!=0){
            (*res).num[count++]=temp.num[i];
            clr=1;
        }
        else if(clr==1){
            (*res).num[count++]=temp.num[i];
        }

    }
    (*res).length=count;
   return ;
}

void bignum_add(struct bignum *res,struct bignum x,struct bignum y)
{
   struct bignum temp;
 //  memset(temp.num,-1,sizeof(temp.num));
   int i,j,k;
   /*
   for(i=0;i<p.length;i++)
      x.num[i]=p.num[i];
      x.length=p.length;
      x.sign=p.sign;
   for(i=0;i<q.length;i++)
      y.num[i]=q.num[i];
      y.length=q.length;
      y.sign=q.sign;
      */
   if(x.length>y.length)
   {
       k=x.length-y.length;
       for(j=x.length;j>=k;j--)
          y.num[j]=y.num[j-k];
       y.length=x.length;
       for(j=0;j<k;j++)
          y.num[j]=0;
   }
   else if(y.length>x.length)
   {
       k=y.length-x.length;
       for(j=y.length;j>=k;j--)
          x.num[j]=x.num[j-k];
       x.length=y.length;
       for(j=0;j<k;j++)
          x.num[j]=0;
   }
  // bignum_print(x);bignum_print(y);
   if(x.sign==y.sign)
   {
       int  e=0;                                       // carry
         for(i=x.length;i>=0;i--)
           {
                temp.num[i+1]=x.num[i]+y.num[i]+e;
                if(temp.num[i+1]>=10)
                {
                     e=temp.num[i+1]/10;
                     temp.num[i+1]%=10;
                }
                else e=0;
           }

         temp.num[0]=e;
         temp.sign=x.sign;
         temp.length=x.length+1;
   }
   else if(x.sign==1)
   {
       int r;
       r=bignum_magnitude(x,y);
       if(r==0)
         {
             temp.sign=0;
             temp.length=1;
             temp.num[0]=0;
         }
       else if(r==1)
       {
           for(i=x.length-1;i>=0;i--)
             {
                if(x.num[i]>=y.num[i])
                   temp.num[i]=x.num[i]-y.num[i];
                else {
                        if(i>=1)
                        x.num[i-1]--;
                        x.num[i]+=10;
                        temp.num[i]=x.num[i]-y.num[i];
                     }
            }
            temp.length=x.length;
            temp.sign=x.sign;
       }
       else{
             for(i=y.length-1;i>=0;i--)
                {
                 if(y.num[i]>=x.num[i])
                   temp.num[i]=y.num[i]-x.num[i];
                else {
                        if(i>=1)
                        y.num[i-1]--;
                        y.num[i]+=10;
                        temp.num[i]=y.num[i]-x.num[i];
                     }
                 }
             temp.length=y.length;
             temp.sign=y.sign;

       }
   }

else
   {
       int r;
       r=bignum_magnitude(x,y);
       if(r==0)
         {
             temp.sign=0;
             temp.length=1;
             temp.num[0]=0;
         }
       else if(r==1)
       {
           for(i=x.length-1;i>=0;i--)
             {
                if(x.num[i]>=y.num[i])
                   temp.num[i]=x.num[i]-y.num[i];
                else {
                        if(i>=1)
                        x.num[i-1]--;
                        x.num[i]+=10;
                        temp.num[i]=x.num[i]-y.num[i];
                     }
            }
            temp.length=x.length;
            temp.sign=x.sign;
       }
       else{
             for(i=y.length-1;i>=0;i--)
                {
                 if(y.num[i]>=x.num[i])
                   temp.num[i]=y.num[i]-x.num[i];
                else {
                        if(i>=1)
                        y.num[i-1]--;
                        y.num[i]+=10;
                        temp.num[i]=y.num[i]-x.num[i];
                     }
                 }
             temp.length=y.length;
             temp.sign=y.sign;

       }
   }

if(temp.length==1)
{
    (*res).sign=temp.sign;
    (*res).length=temp.length;
    (*res).num[0]=temp.num[0];
    return ;
}
int count=0,clr=0;
(*res).sign=temp.sign;
for(i=0;i<temp.length;i++)
    {
        if(temp.num[i]!=0){
            (*res).num[count++]=temp.num[i];
            clr=1;
        }
        else if(clr==1){
            (*res).num[count++]=temp.num[i];
        }

    }
    (*res).length=count;
   return ;
}

void bignum_getnum_pos(struct bignum *res,struct bignum a,int p,int q)
{
    int i,k=p,n=q+1-p;
    for(i=0;i<n;i++)
        (*res).num[i]=a.num[k++];
    (*res).length=n;
    (*res).sign=a.sign;
    return;
}

void bignum_divide(struct bignum *res,struct bignum p,struct bignum q)
{
    if(bignum_magnitude(zero,q)==0)
    {
        printf("\n Divide by zero error ......\n");
        return ;
    }
    if(bignum_magnitude(one,q)==0)
    {
        (*res).length=p.length;
        int i;
        for(i=0;i<p.length;i++)
        (*res).num[i]=p.num[i];
        (*res).sign=(p.sign+q.sign)%2;
        return ;
    }

   (*res).sign=(p.sign+q.sign)%2;
   (*res).length=0;
   int i,j;
   int r=bignum_magnitude(p,q);
   if(r==-1)
   {
       (*res).length=1;
       (*res).num[0]=0;
       return;
   }
   else if(r==0)
   {
       (*res).length=1;
       (*res).num[0]=1;
       return;
   }
   else{
        //  printf("Yeah !\n");
      /*    (*res).length=1;
          (*res).num[0]=0;
          struct bignum temp;
          temp.sign=0;
          temp.num[0]=1;
          temp.length=1;
          while(bignum_magnitude(p,q)==1 || bignum_magnitude(p,q)==0)
          {
              bignum_subtract(&p,p,q);
           //   bignum_print(p);
              bignum_add(res,*res,temp);
            //  bignum_print(*res);
          } */
          int i;
          struct bignum temp;
          temp.length=2;
          temp.num[0]=temp.num[1]=1;
          bignum_getnum_pos(&temp,p,0,q.length-1);
        //  bignum_print(temp);
          i=q.length-1;
         while(i<=p.length-1)
         {
              if(temp.length==1)
             {
                 if(temp.num[0]==0)
                 {
                     (*res).length++;
                     (*res).num[(*res).length-1]=0;
                     temp.length--;
                     i++;
                   //  printf("i am here\n");
                     continue;
                 }

             }
             int z=bignum_magnitude(temp,q);
             if(z==0)
             {
                 (*res).length++;
                 (*res).num[(*res).length-1]=1;
                 temp.length=1;
                 i++;
                 temp.num[temp.length-1]=p.num[i];
                 temp.sign=0;
             }
             else if(z==-1)
             {
                 (*res).length++;
                 (*res).num[(*res).length-1]=0;
                 temp.length++;
                 i++;
                 temp.num[temp.length-1]=p.num[i];
                 temp.sign=0;
             }
             else
             {
                 int count=1;
                struct bignum y,x;
                y.sign=0;x.sign=0;
                y.length=1;x.length=1;
                y.num[0]=0;x.num[0]=0;
                if(temp.num[0]==0)
                {
                   int xx;
           //        printf("YO");
                   for(xx=1;xx<temp.length;xx++)
                   {
                       temp.num[xx-1]=temp.num[xx];
                   }
                   temp.length--;
                 //  bignum_print(temp);
                 (*res).length++;
                (*res).num[(*res).length-1]=0;
                i++;
                continue;
                }
            //    bignum_print(temp);
                while(bignum_magnitude(y,temp)==-1||bignum_magnitude(y,temp)==0)
                {
                    count++;
                    y.length=1;
                    y.num[0]=count;
                    bignum_multiply(&y,q,y);
             //      bignum_print(y);
                }
                (*res).length++;
                (*res).num[(*res).length-1]=count-1;
                y.length=1;
                y.num[0]=count-1;
                bignum_multiply(&x,q,y);
                bignum_subtract(&temp,temp,x);

                if(bignum_magnitude(temp,zero)==0)
                {
                    temp.length=0;
                }
                temp.length++;i++;
                temp.num[temp.length-1]=p.num[i];
                temp.sign=0;
             }

         }
   }
if((*res).num[0]==0)
{
    int xx;
    for(xx=1;xx<(*res).length;xx++)
        (*res).num[xx-1]=(*res).num[xx];
    (*res).length--;
}
 //   bignum_print(*res);
    return ;
}


void bignum_mod(struct bignum *res,struct bignum p,struct bignum q)
{
    struct bignum check;
    check.sign=0;
    check.num[0]=0;
    check.length=1;
    if(bignum_magnitude(check,q)==0)
    {
        printf("\n Mod by zero error ......\n");
        return ;
    }
    check.num[0]=1;
    if(bignum_magnitude(check,q)==0)
    {
        (*res).num[0]=0;
        (*res).length=1;
        (*res).sign=0;
        return;
    }
    if(bignum_magnitude(p,q)==0)
    {
        (*res).num[0]=0;
        (*res).length=1;
        (*res).sign=0;
        return;
    }
    if(bignum_magnitude(p,q)==-1)
    {
        int i;
        for(i=0;i<p.length;i++)
            (*res).num[i]=p.num[i];
        (*res).length=p.length;
        (*res).sign=p.sign;
        return;
    }
    struct bignum temp;
    bignum_divide(&temp,p,q);
    bignum_multiply(&temp,temp,q);
    bignum_subtract(&temp,p,temp);
    int i;
    for(i=0;i<temp.length;i++)
            (*res).num[i]=temp.num[i];
        (*res).length=temp.length;
        (*res).sign=temp.sign;
        return;
}

void bignum_gcd(struct bignum *res,struct bignum p,struct bignum q)
{
    struct bignum check;
    check.sign=0;
    check.num[0]=0;
    check.length=1;
    if(bignum_magnitude(check,q)==0)
    {
        int i;
        for(i=0;i<p.length;i++)
            (*res).num[i]=p.num[i];
        (*res).length=p.length;
        (*res).sign=p.sign;
        return;
    }
    if(bignum_magnitude(check,p)==0)
    {
        int i;
        for(i=0;i<q.length;i++)
            (*res).num[i]=q.num[i];
        (*res).length=q.length;
        (*res).sign=q.sign;
        return;
    }
    check.num[0]=1;
   if(bignum_magnitude(check,p)==0 || bignum_magnitude(check,q)==0)
    {
        (*res).num[0]=1;
        (*res).length=1;
        (*res).sign=0;
        return;
    }
   struct bignum c;int i;
   check.num[0]=0;
   while(bignum_magnitude(p,check)!=0)
   {
       for(i=0;i<p.length;i++)
          c.num[i]=p.num[i];
       c.length=p.length;
       c.sign=p.sign;
       bignum_mod(&p,q,p);
       for(i=0;i<c.length;i++)
          q.num[i]=c.num[i];
       q.length=c.length;
       q.sign=c.sign;
   }
   for(i=0;i<q.length;i++)
    (*res).num[i]=q.num[i];
   (*res).length=q.length;
   (*res).sign=q.sign;
   return;
}
void bignum_mul_inv(struct bignum *res,struct bignum a,struct bignum b)
{
    struct bignum b0,t,q,x0,x1,temp;
    int i;
    for(i=0;i<b.length;i++)
    {
        b0.num[i]=b.num[i];
    }
    b0.length=b.length;
    b0.sign=b.sign;
    x0.num[0]=0;
    x0.length=1;
    x0.sign=0;
    x1.num[0]=1;
    x1.length=1;
    x1.sign=0;
    if(bignum_magnitude(b,one)==0)
    {
        (*res).num[0]=1;
        (*res).length=1;
        (*res).sign=0;
        return;
    }
    while(bignum_magnitude(a,one)==1)
    {
        bignum_divide(&q,a,b);
        for(i=0;i<b.length;i++)
            t.num[i]=b.num[i];
        t.length=b.length;
        t.sign=b.sign;
        bignum_mod(&b,a,b);
        for(i=0;i<t.length;i++)
            a.num[i]=t.num[i];
        a.length=t.length;
        a.sign=t.sign;
        for(i=0;i<x0.length;i++)
            t.num[i]=x0.num[i];
        t.length=x0.length;
        t.sign=x0.sign;
        bignum_multiply(&temp,q,x0);
        bignum_subtract(&x0,x1,temp);
        for(i=0;i<t.length;i++)
            x1.num[i]=t.num[i];
        x1.length=t.length;
        x1.sign=t.sign;
      //  bignum_print(a);bignum_print(b);bignum_print(t);bignum_print(x0);bignum_print(x1);bignum_print(q);
    }
    if(bignum_compare(x1,zero)==-1)
    {
        bignum_add(&x1,x1,b0);
    }
    for(i=0;i<x1.length;i++)
         (*res).num[i]=x1.num[i];
       (*res).length=x1.length;
       (*res).sign=x1.sign;
    return;
}

void bignum_div_2(struct bignum *res,struct bignum x)
{
    int l=x.length,i,count=0;
    if(x.num[0]==0 || x.num[0]==1)
        if(x.length==1)
    {
        (*res).length=1;
        (*res).num[0]=0;
        (*res).sign=x.sign;
        return;
    }
    struct bignum temp;
    for(i=0;i<l;i++)
    {
        if(x.num[i]==0)
          {
             temp.num[count++]=0;
          }
       else if(x.num[i]==1)
        {
            x.num[i+1]+=10;
            if(i!=0)
                temp.num[count++]=0;
            continue;
        }
        else if(x.num[i]%2==0)
            temp.num[count++]=x.num[i]/2;
        else {
            temp.num[count++]=x.num[i]/2;
            x.num[i+1]+=10;
        }
    }
    for(i=0;i<count;i++)
        (*res).num[i]=temp.num[i];
    (*res).length=count;
    (*res).sign=x.sign;
    return;
}

void bignum_mod_exp(struct bignum *res,struct bignum m,struct bignum n,struct bignum mod)
{
   struct bignum x,prod,temp;
   int i;
   prod.sign=0;
   prod.length=1;
   prod.num[0]=1;
   x.length=n.length;
   x.sign=n.sign;
   for(i=0;i<n.length;i++)
     x.num[i]=n.num[i];
   while(bignum_magnitude(x,zero)>0)
   {
       if(bignum_even(x)==1)
       {
           bignum_multiply(&prod,prod,m);
         //  bignum_print(prod);
           bignum_mod(&prod,prod,mod);
         //  bignum_print(prod);
       }
       bignum_multiply(&temp,m,m);
     //  bignum_print(temp);
       bignum_mod(&m,temp,mod);
       bignum_div_2(&x,x);
     //  bignum_print(x);
      // bignum_print(prod);
   }
          (*res).length=prod.length;
           (*res).sign=prod.sign;
           for(i=0;i<prod.length;i++)
              (*res).num[i]=prod.num[i];
    return;
}

void bignum_input(struct bignum *res)
{
    char a[MAXLENGTH+1];
    scanf("%s",a); int i;
    (*res).length=strlen(a);
    (*res).sign=0;
    if(a[0]=='-')
     {
          (*res).length--;
          (*res).sign=1;
     }
     if(a[0]=='-')
     {
        for(i=0;i<(*res).length;i++)
            (*res).num[i]=a[i+1]-48;                      // Convert char to int
     }
     else
     {
         for(i=0;i<(*res).length;i++)
            (*res).num[i]=a[i]-48;                      // Convert char to int
     }
    return;
}

void bignum_input_str(struct bignum *res,char *a)
{
   // char a[MAXLENGTH+1];
    //scanf("%s",a);
     int i;
    (*res).length=strlen(a);
    (*res).sign=0;
    if(a[0]=='-')
     {
          (*res).length--;
          (*res).sign=1;
     }
     if(a[0]=='-')
     {
        for(i=0;i<(*res).length;i++)
            (*res).num[i]=a[i+1]-48;                      // Convert char to int
     }
     else
     {
         for(i=0;i<(*res).length;i++)
            (*res).num[i]=a[i]-48;                      // Convert char to int
     }
    return;
}


int main()
{
    zero.sign=0;
    zero.length=1;
    zero.num[0]=0;
    one.sign=0;
    one.length=1;
    one.num[0]=1;
    struct bignum p,q,phi,n,e,c,d,m,mi;
    printf("\n Enter p : ");
    bignum_input_str(&p,"987999845231236574123598452312365741235985719996663381");
   // bignum_print(p);
    printf("\n Enter q : ");
    bignum_input_str(&q,"987999845231236574123598564987999845231236574123598593");
    bignum_print(p);bignum_print(q);
    bignum_multiply(&n,p,q);
    bignum_subtract(&p,p,one);
    bignum_subtract(&q,q,one);
    bignum_multiply(&phi,p,q);
    printf("\n n = ");
    bignum_print(n);
    printf("\n phi = ");
    bignum_print(phi);
    printf("\n Enter e : ");
    bignum_input_str(&e,"17");
    bignum_mul_inv(&d,e,phi);
    printf("\n d = ");
    bignum_print(d);
    printf("\n Enter m (message) : ");
    bignum_input_str(&m,"123456789987654321123456789");
    bignum_mod_exp(&c,m,e,n);
    printf("\n c (cipher) = ");
    bignum_print(c);
    bignum_mod_exp(&mi,c,d,n);
    printf("\n mi (message) = ");
    bignum_print(mi);
 //   getchar();
    return 0;
}
