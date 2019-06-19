//IDE: Dev-C++ 5.7.1 

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

// структура, описывающая запись в базе данных
struct record                                          //convert HEX->DEC
{
	char author[12];    //12
	char title[32]; 
	char publisher[16]; 
	short int year;   //int     or    uint     or     uint32
	short int pages;
};

// структура, описывающая элемент списка
struct item
{
    struct record data; 	// информация из базы данных
    struct item* next;
};

// добавление нового элемента при загрузке в оперативную память компьютера
struct item* add_item(struct record& data)
{
	struct item* item = (struct item*)malloc(sizeof(struct item));
	item->data = data;
	item->next = NULL;
	
	return item;	
}

// структура, описывающая вершину дерева
struct node
{
    struct record data; 	// информация из базы данных
    int height;				// высота поддерева
    char key[4];			// ключ поиска
    struct node* next;		// указатель на следующую вершину в очереди
    struct node* left;  	// указатель на левого потомка
    struct node* right; 	// указатель на правого потомка
};

// выделение памяти для хранения вершины и ее инициализация
struct node* create_node(struct record& data, char* initials)
{ 
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->data = data;
    node->height = 1;
    strcpy(node->key, initials);
    node->next = NULL;
    node->left = NULL;
    node->right = NULL;
 
	return node;
}

// определяет высоту дерева
int height(struct node* node)
{
	if (node == NULL)
    	return 0;
    return node->height;
}
 
// определяет максимальное значение из двух
int max(int a, int b)
{
	return (a > b) ? a : b;
}

// поворот направо
struct node* right_rotate(struct node* root)
{
    struct node* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    root->height = max(height(root->left), height(root->right)) + 1;
    new_root->height = max(height(new_root->left), height(new_root->right)) + 1;
 
    return new_root;
}
 
// поворот налево
struct node* left_rotate(struct node* root)
{
    struct node* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    root->height = max(height(root->left), height(root->right)) + 1;
    new_root->height = max(height(new_root->left), height(new_root->right)) + 1;
 
    return new_root;
}

// вычисление показателя балансировки
int get_balance(struct node* node)
{
	if (node == NULL)
    	return 0;
    return height(node->left) - height(node->right);
}

// вставка вершины в АВЛ-дерево
struct node* avl_insert(struct node* node, struct record& data, char* initials)
{
	// вставка нового узла
    if (node == NULL)
        return(create_node(data, initials));
    
    if (strcmp(initials, node->key) < 0)
    	node->left = avl_insert(node->left, data, initials);
	else if (strcmp(initials, node->key) > 0)
        node->right = avl_insert(node->right, data, initials);
    else
    {
    	// вершины с одинаковыми ключами образуют очередь
    	struct node* temp = node;
    	
    	while(temp->next != NULL)
    		temp = temp->next;
    	
    	temp->next = create_node(data, initials);
    }
 
    // обновление высоты родительского узла
    node->height = max(height(node->left), height(node->right)) + 1;
 
    // проверка на балансировку
    int balance = get_balance(node);
 
    // в случае разбалансировки получаются 4 возможные ситуации:
    // LL-поворот
    if (balance > 1 && strcmp(initials, node->left->key) < 0)
    	return right_rotate(node);
 
    // RR-поворот
    if (balance < -1 && strcmp(initials, node->right->key) > 0)
        return left_rotate(node);
 
    // LR-поворот
    if (balance > 1 && strcmp(initials, node->left->key) > 0)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
 
    // RL-поворот
    if (balance < -1 && strcmp(initials, node->right->key) < 0)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    
    return node;
}

// определение инициалов для поиска по ключу
void get_initials(char* full_name, char* initials)
{
	char* name;
	int count = 0;
	
	*initials = *full_name;
	count++;
	
	for(name = ++full_name; *name; name++)
	{
		if(*name == 32)
		{	
			*++initials = *(name + sizeof(char));
			count++;
			
			if(count == 3) 
				break;
		}
	}
}

// поиск вершины с заданным ключом
struct node* node_search(struct node* node, char* key, int& count)
{
	// достигнута терминальная вершина
    if(node == NULL)
    	return NULL;
	
	// ключ поиска больше ключа данных вершины
    if(strcmp(node->key, key) < 0) 						
    	return node_search(node->right, key, count);
    // ключ поиска меньше ключа данных вершины
 	else if(strcmp(node->key, key) > 0)					
    	return node_search(node->left, key, count);
    // найдена вершина с заданным ключом
    else
    {
    	// вывод информации из первой вершины очереди
    	count++;
    	printf("%-4d%-13s%-32s%-16s%-5d%-5d\n", count, node->data.author, node->data.title, 
			node->data.publisher, node->data.year, node->data.pages);
    	
    	// вывод информации из последующих вершин в очереди
		struct node* temp = node;
    	
    	while(temp->next != NULL)
    	{
    		count++;
    		temp = temp->next;
    		printf("%-4d%-13s%-32s%-16s%-5d%-5d\n", count, node->data.author, node->data.title, 
				node->data.publisher, node->data.year, node->data.pages);
    	}
    	
    	if(node->right != NULL && (node->right->key, key) == 0)
			node_search(node->right, key, count);
			
		if(node->left != NULL &&  strcmp(node->left->key, key) == 0)
			node_search(node->left, key, count);
			
		return node;
    }
}

// загружает записи из базы данных в оперативную память
struct item* load_database(struct item* head, bool& loaded)
{
	if(loaded)
	{
		printf("\n\nDatabase is in RAM!\n");
		system("pause");
	}
	else
	{
		FILE* file; // указатель на файл базы данных
		struct record prominent;
		struct item* tail = NULL;
		struct item* new_item = NULL;
		
		if((file = fopen("base1.dat", "rb")) == NULL) // если указанный файл существует, его следует открыть для чтения
		{
			printf("\n\nDatabase file does not exist!\n"); // иначе выводится сообщение об ошибке
			system("pause");
			
			return head;
		}
		else
		{
			// проходит по файлу до конца
			while(!feof(file))
			{
				// считывает информацию в структуру
				fread(&prominent, sizeof(struct record), 1, file);
			
				// записи сохраняются в оперативной памяти в виде односвязного списка
				if(head == NULL)
				{
					head = tail = add_item(prominent);
				}
				else
				{
					new_item = add_item(prominent);
					tail->next = new_item;
					tail = new_item;
				}
			}
			
			fclose(file); //закрытие файла
			loaded = true;
		}
	
		printf("\n\nDatabase has been loaded.\n");
		system("pause");		
	}
	
	return head;
}

// строит АВЛ-дерево, упорядоченное по заданному полю
void create_avl(struct item* head, bool loaded, int order)
{
	if(loaded)
	{
		struct node* avl_root = NULL;
		struct item* current = head;
	
		// построение АВЛ-дерева из данных в оперативной памяти
		while(current != NULL)
		{
			current->data.author[12] = '\0';
			current->data.publisher[15] = '\0';
			
			char initials[4] = "   "; 
    		
			if(order == 1)
				get_initials(current->data.author, initials);
			else
				get_initials(current->data.title, initials);
			
			avl_root = avl_insert(avl_root, current->data, initials);
			current = current->next;
		}
		
		system("cls");
		
		if(order == 1)
			printf("   *** AVL-TREE ORDERED BY FIELD - \"AUTHOR\" ***\n\n");
		else
			printf("   *** AVL-TREE ORDERED BY FIELD - \"TITLE\" ***\n\n");
		printf(" The AVL-tree has been built.\n\n");
		
		// поиск вершин по ключу, состоящему из трех заглавных букв русского алфавита
		printf("SEARCH OPERATION allows to look for specific keys.\n");
		printf("Use three capital letters of the russian alphabet.\n\n");
		printf(" Search for: ");
		
		int count = 0;
		char key[4];
		gets(key);
		
		if(node_search(avl_root, key, count) == NULL)
			printf("\nNo records with key \"%s\" found!\n", key);
	
		printf("\n");
		
		system("pause");
	}
	else
	{
		system("cls");
		printf(" ERROR! No data available.\n\n");
		printf("You'll need to load database first!\n");
		printf("Proceed to main menu and press 1.\n");
		system("pause");	
	}
}

// реализация меню программы
void menu()
{
	system("cls");
	printf("                   COURSE PROJECT");
	printf("\n Variant #1");
	printf("\n\nThis program will create AVL-trees and perform search");
	printf("\noperations using database - \"Life of Prominent People\".");
	printf("\n\nSelect command:");
	printf("\n\n Press 1 to load database into memory");
	printf("\n Press 2 to build an AVL-tree ordered by the first field");
	printf("\n Press 3 to build an AVL-tree ordered by the second field");
	printf("\n Press Esc to quit. ");
}

int main()
{
	char key;
	bool exit = false;
	bool loaded = false;
	struct item* head = NULL;
	
	menu();
	
	while(!exit)
	{
		key = getch();
		
		switch((unsigned char)key)
		{
			case 49: head = load_database(head, loaded); break;
			case 50: create_avl(head, loaded, 1); break;
			case 51: create_avl(head, loaded, 2); break;
			case 27: exit = true; break;
			default: break;
		}
		
		menu();
	}
 	
    return 0;
}
