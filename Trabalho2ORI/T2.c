/*From
https://sites.google.com/site/itstudentjunction/lab-programming-solutions/data-structures-programs/trees--programs---data-structures/program-for-implementation-of-b-tree-insertion--deletion
*/

#include <stdio.h>
#include <stdlib.h>
#define M 3

struct node {
    int n; /* n < M No. of keys in node will always less than order of B tree */
    int keys[M-1]; /*array of keys*/
    struct node *p[M]; /* (n+1 pointers will be in use) */
}*root=NULL;

enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

void insert(int key);
void display(struct node *root,int);
void DelNode(int x);
void search(int x);
enum KeyStatus ins(struct node *r, int x, int* y, struct node** u);
int searchPos(int x,int *key_arr, int n);
enum KeyStatus del(struct node *r, int x);
void eatline(void);
void inorder(struct node *ptr);
int totalKeys(struct node *ptr);
void printTotal(struct node *ptr);
int getMin(struct node *ptr);
int getMax(struct node *ptr);
void getMinMax(struct node *ptr);
int max(int first, int second, int third);
int maxLevel(struct node *ptr);
void printMaxLevel(struct node *ptr);
void getNodeDisco(int posicao,struct node *ptr );
void setNodeDisco(struct node* ptr);


int main()
{
    int key;
    int choice;
    printf("Creation of B tree for M=%d\n",M);
    while(1)
    {
        printf("1.Insert\n");
        printf("2.Delete\n");
        printf("3.Search\n");
        printf("4.Display\n");
        printf("5.Quit\n");
        printf("6.Enumerate\n");
        printf("7.Total Keys\n");
        printf("8.Min and Max Keys\n");
        printf("9.Max Level\n");
        printf("Enter your choice : ");
        scanf("%d",&choice); eatline();

        switch(choice)
        {
        case 1:
            printf("Enter the key : ");
            scanf("%d",&key); eatline();
            insert(key);                                                                    //Adiciona uma nova chave
            break;
        case 2:
            printf("Enter the key : ");
            scanf("%d",&key); eatline();
            DelNode(key);
            break;
        case 3:
            printf("Enter the key : ");
            scanf("%d",&key); eatline();
            search(key);
            break;
        case 4:
            printf("Btree is :\n");
            display(root,0);
            break;
        case 5:
            exit(1);
        case 6:
            printf("Btree in sorted order is:\n");
            inorder(root); putchar('\n');
            break;
        case 7:
        	printf("The total number of keys in this tree is:\n");
            printTotal(root);
            break;
        case 8:
            getMinMax(root); 
            break;
        case 9:
        	printf("The maximum level in this tree is:\n");
        	printMaxLevel(root);
        	break;
        default:
            printf("Wrong choice\n");
            break;
        }/*End of switch*/
    }/*End of while*/
    return 0;
}/*End of main()*/

void insert(int key)
{
    struct node *newnode;                                                           //Cria um novo nó
    int upKey;                                                                      //Inteiro paraa chave que foi promovida
    enum KeyStatus value;                                                           //Enum com o status da insercao
    value = ins(root, key, &upKey, &newnode);
    if (value == Duplicate)                                                         //Caso a key já está na arvore
        printf("Key already available\n");
    if (value == InsertIt)                                                          //Caso de inserir, pois uma chave foi promovida
    {
        struct node *uproot = root;                                                 //Novo nó gerado pela divisao
        root=malloc(sizeof(struct node));                                           //Alocação de memória para o no raiz
        root->n = 1;                                                                //Numero de chaves dentro do nó
        root->keys[0] = upKey;                                                      //Insercao da chave promovida
        root->p[0] = uproot;                                                        //Aponta para o antigo nó
        root->p[1] = newnode;                                                       //Aponta para o novo nó    
    }/*End of if */
}/*End of insert()*/

enum KeyStatus ins(struct node *ptr, int key, int *upKey,struct node **newnode)
{
    struct node *newPtr, *lastPtr;
    int pos, i, n,splitPos;
    int newKey, lastKey;
    enum KeyStatus value;
    if (ptr == NULL)                                                               //Caso não tenha um nó
    {
        *newnode = NULL;                                                           //Não haverá um novo nó
        *upKey = key;                                                              //Promovido a própria chave
        return InsertIt;
    }
    n = ptr->n;                                                                    //Pega a quantidade de keys dentro do nó
    pos = searchPos(key, ptr->keys, n);                                            //Procura a key dentro da arvore
    if (pos < n && key == ptr->keys[pos])                                          //Caso em que a chave já está na arvore 
        return Duplicate;
    value = ins(ptr->p[pos], key, &newKey, &newPtr);                               //Chamada recursiva
    if (value != InsertIt)                                                         //Já inserido e tratado os casos
        return value;
    /*If keys in node is less than M-1 where M is order of B tree*/
    if (n < M - 1)                                                                 //Caso em que é possivel adicionar uma key
    {
        pos = searchPos(newKey, ptr->keys, n);                                     //Procura novamente
        /*Shifting the key and pointer right for inserting the new key*/
        for (i=n; i>pos; i--)                                                      //For para organizar as ket em ordem
        {
            ptr->keys[i] = ptr->keys[i-1];
            ptr->p[i+1] = ptr->p[i];
        }
        /*Key is inserted at exact location*/
        ptr->keys[pos] = newKey;                                                   //Adiciona a ket
        ptr->p[pos+1] = newPtr;                                                    //Adiciona um ponteiro
        ++ptr->n; /*incrementing the number of keys in node*/                      //Incrementa a quantidade de keys
        return Success;
    }/*End of if */
    /*If keys in nodes are maximum and position of node to be inserted is last*/
    if (pos == M - 1)                                                      //Caso o nó esteja cheio e a ulttima key deve ser ins
    {
        lastKey = newKey;
        lastPtr = newPtr;                                                   
    }
    else /*If keys in node are maximum and position of node to be inserted is not last*/
    {
        lastKey = ptr->keys[M-2];                                               //Pega a penultima chave
        lastPtr = ptr->p[M-1];                                                  //Pega o penultima ponteiro
        for (i=M-2; i>pos; i--)                                                 //Passa as chaves para a posicao da frente
        {
            ptr->keys[i] = ptr->keys[i-1];
            ptr->p[i+1] = ptr->p[i];
        }
        ptr->keys[pos] = newKey;                                                //Recebe a chave
        ptr->p[pos+1] = newPtr;                                                 //recebe o ponteiro
    }
    splitPos = (M - 1)/2;                                                       //Verificação da posicao da divisao
    (*upKey) = ptr->keys[splitPos];                                             //Chave a ser promovida dada a divisao

    (*newnode)=malloc(sizeof(struct node));/*Right node after split*/
    ptr->n = splitPos; /*No. of keys for left splitted node*/
    (*newnode)->n = M-1-splitPos;/*No. of keys for right splitted node*/
    for (i=0; i < (*newnode)->n; i++)                                           //For para adicionar as key/ponteiros no newNode
    {
        (*newnode)->p[i] = ptr->p[i + splitPos + 1];
        if(i < (*newnode)->n - 1)
            (*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
        else
            (*newnode)->keys[i] = lastKey;                                     //Adiciona a ultima chave
    }
    (*newnode)->p[(*newnode)->n] = lastPtr;                                    //Adiciona o ultimo ponteiro
    return InsertIt;
}/*End of ins()*/

void display(struct node *ptr, int blanks)
{
    if (ptr)
    {
        int i;
        for(i=1; i<=blanks; i++)
            printf(" ");
        for (i=0; i < ptr->n; i++)
            printf("%d ",ptr->keys[i]);
        printf("\n");
        for (i=0; i <= ptr->n; i++)
            display(ptr->p[i], blanks+10);
    }/*End of if*/
}/*End of display()*/

void search(int key)
{
    int pos, i, n;
    struct node *ptr = root;
    printf("Search path:\n");
    while (ptr)
    {
        n = ptr->n;
        for (i=0; i < ptr->n; i++)
            printf(" %d",ptr->keys[i]);
        printf("\n");
        pos = searchPos(key, ptr->keys, n);
        if (pos < n && key == ptr->keys[pos])
        {
            printf("Key %d found in position %d of last dispalyed node\n",key,i);
            return;
        }
        ptr = ptr->p[pos];
    }
    printf("Key %d is not available\n",key);
}/*End of search()*/

int searchPos(int key, int *key_arr, int n)
{
    int pos=0;
    while (pos < n && key > key_arr[pos])
        pos++;
    return pos;
}/*End of searchPos()*/

void DelNode(int key)
{
    struct node *uproot;
    enum KeyStatus value;
    value = del(root,key);
    switch (value)
    {
    case SearchFailure:
        printf("Key %d is not available\n",key);
        break;
    case LessKeys:
        uproot = root;
        root = root->p[0];
        free(uproot);
        break;
    }/*End of switch*/
}/*End of delnode()*/

enum KeyStatus del(struct node *ptr, int key)
{
    int pos, i, pivot, n ,min;
    int *key_arr;
    enum KeyStatus value;
    struct node **p,*lptr,*rptr;

    if (ptr == NULL)
        return SearchFailure;
    /*Assigns values of node*/
    n=ptr->n;
    key_arr = ptr->keys;
    p = ptr->p;
    min = (M - 1)/2;/*Minimum number of keys*/

    //Search for key to delete
    pos = searchPos(key, key_arr, n);
    // p is a leaf
    if (p[0] == NULL)
    {
        if (pos == n || key < key_arr[pos])
            return SearchFailure;
        /*Shift keys and pointers left*/
        for (i=pos+1; i < n; i++)
        {
            key_arr[i-1] = key_arr[i];
            p[i] = p[i+1];
        }
        return --ptr->n >= (ptr==root ? 1 : min) ? Success : LessKeys;
    }/*End of if */

    //if found key but p is not a leaf
    if (pos < n && key == key_arr[pos])
    {
        struct node *qp = p[pos], *qp1;
        int nkey;
        while(1)
        {
            nkey = qp->n;
            qp1 = qp->p[nkey];
            if (qp1 == NULL)
                break;
            qp = qp1;
        }/*End of while*/
        key_arr[pos] = qp->keys[nkey-1];
        qp->keys[nkey - 1] = key;
    }/*End of if */
    value = del(p[pos], key);
    if (value != LessKeys)
        return value;

    if (pos > 0 && p[pos-1]->n > min)
    {
        pivot = pos - 1; /*pivot for left and right node*/
        lptr = p[pivot];
        rptr = p[pos];
        /*Assigns values for right node*/
        rptr->p[rptr->n + 1] = rptr->p[rptr->n];
        for (i=rptr->n; i>0; i--)
        {
            rptr->keys[i] = rptr->keys[i-1];
            rptr->p[i] = rptr->p[i-1];
        }
        rptr->n++;
        rptr->keys[0] = key_arr[pivot];
        rptr->p[0] = lptr->p[lptr->n];
        key_arr[pivot] = lptr->keys[--lptr->n];
        return Success;
    }/*End of if */
//if (posn > min)
    if (pos < n && p[pos + 1]->n > min)
    {
        pivot = pos; /*pivot for left and right node*/
        lptr = p[pivot];
        rptr = p[pivot+1];
        /*Assigns values for left node*/
        lptr->keys[lptr->n] = key_arr[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
        key_arr[pivot] = rptr->keys[0];
        lptr->n++;
        rptr->n--;
        for (i=0; i < rptr->n; i++)
        {
            rptr->keys[i] = rptr->keys[i+1];
            rptr->p[i] = rptr->p[i+1];
        }/*End of for*/
        rptr->p[rptr->n] = rptr->p[rptr->n + 1];
        return Success;
    }/*End of if */

    if(pos == n)
        pivot = pos-1;
    else
        pivot = pos;

    lptr = p[pivot];
    rptr = p[pivot+1];
    /*merge right node with left node*/
    lptr->keys[lptr->n] = key_arr[pivot];
    lptr->p[lptr->n + 1] = rptr->p[0];
    for (i=0; i < rptr->n; i++)
    {
        lptr->keys[lptr->n + 1 + i] = rptr->keys[i];
        lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
    }
    lptr->n = lptr->n + rptr->n +1;
    free(rptr); /*Remove right node*/
    for (i=pos+1; i < n; i++)
    {
        key_arr[i-1] = key_arr[i];
        p[i] = p[i+1];
    }
    return --ptr->n >= (ptr == root ? 1 : min) ? Success : LessKeys;
}/*End of del()*/

void eatline(void) {
  char c;
  printf("");
  while (c=getchar()!='\n') ;
}

/* Function to display each key in the tree in sorted order (in-order traversal)
    @param struct node *ptr, the pointer to the node you are currently working with
    */
void inorder(struct node *ptr) {
    if (ptr) {
        if (ptr->n >= 1) {
            inorder(ptr->p[0]);
            printf("%d ", ptr->keys[0]);
            inorder(ptr->p[1]);
            if (ptr->n == 2) {
                printf("%d ", ptr->keys[1]);
                inorder(ptr->p[2]);
            }
        }
    }
}

/* Function that returns the total number of keys in the tree.
    @param struct node *ptr, the pointer to the node you are currently working with
    */
int totalKeys(struct node *ptr) {
    if (ptr) {
        int count = 1;
        if (ptr->n >= 1) {
            count += totalKeys(ptr->p[0]);
            count += totalKeys(ptr->p[1]);
            if (ptr->n == 2) count += totalKeys(ptr->p[2]) + 1;
        }
        return count;
    }
    return 0;
}

/* Function that prints the total number of keys in the tree.
	@param struct node *ptr, the pointer to the node you are currently working with
	*/
void printTotal(struct node *ptr) {
	printf("%d\n",totalKeys(ptr));
}

/* Function that returns the smallest key found in the tree.
    @param struct node *ptr, the pointer to the node you are currently working with
    */
int getMin(struct node *ptr) {
    if (ptr) {
        int min;
        if (ptr->p[0] != NULL) min = getMin(ptr->p[0]);
        else min = ptr->keys[0];
        return min;
    }
    return 0;
}

/* Function that returns the largest key found in the tree.
    @param struct node *ptr, the pointer to the node you are currently working with
    */
int getMax(struct node *ptr) {
    if (ptr) {
        int max;
        if (ptr->n == 1) {
            if (ptr->p[1] != NULL) max = getMax(ptr->p[1]);
            else max = ptr->keys[0];
        }
        if (ptr->n == 2) {
            if (ptr->p[2] != NULL) max = getMax(ptr->p[2]);
            else max = ptr->keys[1];
        }
        return max;
    }
    return 0;
}

/* Function that prints the smallest and largest keys found in the tree.
    @param struct node *ptr, the pointer to the node you are currently working with
    */
void getMinMax(struct node *ptr) {
    printf("%d %d\n", getMin(ptr), getMax(ptr));
}

/* Function that determines the largest number.
	@param int, integer to compare.
	@param int, integer to compare.
	@param int, integer to compare.
	*/
int max(int first, int second, int third) {
	int max = first;
	if (second > max) max = second;
	if (third > max) max = third;
	return max;
}

/*Function that finds the maximum level in the node and returns it as an integer.
	@param struct node *ptr, the node to find the maximum level for.
	*/
int maxLevel(struct node *ptr) {
	if (ptr) {
		int l = 0, mr = 0, r = 0, max_depth;
		if (ptr->p[0] != NULL) l = maxLevel(ptr->p[0]);
		if (ptr->p[1] != NULL) mr = maxLevel(ptr->p[1]);	
		if (ptr->n == 2) {
			if (ptr->p[2] != NULL) r = maxLevel(ptr->p[2]);
		}
		max_depth = max(l, mr, r) + 1;
		return max_depth;
	}
	return 0;
}

/*Function that prints the maximum level in the tree.
	@param struct node *ptr, the tree to find the maximum level for.
	*/
void printMaxLevel(struct node *ptr) {
	int max = maxLevel(ptr) - 1;
	if (max == -1) printf("tree is empty\n");
	else printf("%d\n", max);
}

void getNodeDisco(int posicao,struct node *ptr ){
    struct node *aux;
    int i = 0;
    
    aux = malloc(64);
    memset(aux,0,64);
    
    FILE* arq = fopen("arq.bin", "r+b");
    fseek(arq, posicao, SEEK_SET);
    fread(aux, 64, 1,arq);
    ptr->n = aux->n;
    
    for(i=0;i<aux->n;i++){
        ptr->keys[i] = aux->keys[i];
        ptr->p[i]= aux->p[i];
    }
    ptr->p[aux->n]= aux->p[aux->n];
    free(aux);
    fclose(arq);
    
}

void setNodeDisco(struct node* ptr) {
    int i=0;
    struct node* ptr2;
    ptr2 = (struct node) malloc(64);
    memset(ptr2,0,64);
    ptr2->n = ptr->n;
    for (i = 0; i < ptr->n; i++) {
        ptr2->keys[i] = ptr->keys[i]
        ptr2->*p[i] = ptr->*p[i]
    }
    ptr2->p[ptr2->n] = ptr->p[ptr->n]
    FILE* disk = fopen(disco.c_str(), "w+");
    fwrite(ptr2 , 64 , 1 , disk );
    free(ptr2);
    fclose(disk);
}
