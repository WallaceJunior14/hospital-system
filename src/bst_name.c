#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst_name.h"
typedef struct _patient 
{
    unsigned int id;
    char name[20];
    int age;
    char medical_condition[30];
    struct _patient *left;
    struct _patient *right;
} PatientNodeName;

PatientNodeName *createNodeByName(const unsigned int id, const char *name, const unsigned int age, const char *medical_condition)
{
    PatientNodeName *node = (PatientNodeName*)malloc(sizeof(PatientNodeName));

    if (!node){
        printf("Failed to create a new patient.\n");
        return NULL;
    }

    node->id = id;
    strcpy(node->name, name);
    node->age = age;
    strcpy(node->medical_condition, medical_condition);
    node->left = node->right = NULL;
    
    return node;
}

PatientNodeName *insertNodeByName(PatientNodeName *root, const unsigned int id, const char *name, const unsigned int age, const char *medical_condition)
{
    if (root == NULL){
        root = createNodeByName(id, name, age, medical_condition);

    } else if (strcmp(name, root->name) <= 0){
        root->left = insertNodeByName(root->left, id, name, age, medical_condition);

    } else {
        root->right = insertNodeByName(root->right, id, name, age, medical_condition);
    }    
    return root;
}

PatientNodeName *searchNodeByName(PatientNodeName *root, const char *name)
{
    if (root == NULL || strcmp(root->name, name) == 0){
        return root;
    
    } else if (strcmp(name, root->name) < 0){
        return searchNodeByName(root->left, name);

    } else {
        return searchNodeByName(root->right, name);
    }
}

PatientNodeName *findMinName(PatientNodeName *root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

PatientNodeName *deleteNodeByName(PatientNodeName *root, const char *name)
{
    // Case 1: the node is a leaf
    if (root == NULL ) return root;

    // Case 2: the node has only one son
    if (strcmp(name, root->name) < 0) {
        root->left = deleteNodeByName(root->left, name);

    } else if (strcmp(name, root->name) > 0){
        root->right = deleteNodeByName(root->right, name);

    } else { // Case 3: the node has two sons
        if (root->left == NULL){
            PatientNodeName *temp = root->right;
            free(root);
            return temp;

        } else if (root->right == NULL) {
            PatientNodeName *temp = root->left;
            free(root);
            return temp;
        }
        
        // Finds the minimum value in the right subtree
        PatientNodeName *temp = findMinName(root->right);

        // Copy the minimum node's values to the current node
        root->id = temp->id;
        strcpy(root->name, temp->name);
        root->age = temp->age;
        strcpy(root->medical_condition, temp->medical_condition);

        // Delete the duplicate node in the right subtree
        root->right = deleteNodeByName(root->right, temp->name);
    }
    return root;
}

void traverseName(const PatientNodeName *root)
{
    if (root != NULL)
    {
        traverseName(root->left);
        printf("%s ", root->name);
        traverseName(root->right);
    }
} 

void deallocateByName(PatientNodeName **root)
{
    if (*root == NULL)
        return;
    deallocateByName(&((*root)->left)); 
    deallocateByName(&((*root)->right));  
    free(*root);                       
    *root = NULL;                      
}

void generate_output_file_from_name_tree(PatientNodeName *root)
{
    if (root != NULL)
    {
        // First, traverse the left side of the tree
        generate_output_file_from_name_tree(root->left);

        // Then, process the current root
        FILE *arq_log = fopen("./output/log_root_name.txt", "a");
        if (arq_log == NULL){
            perror("Error opening the file.");
            return;
        }
        
        fprintf(arq_log, "%d %s %d %s\n", root->id, root->name, root->age, root->medical_condition);

        fclose(arq_log);

        // Finally, traverse the right side of the tree
        generate_output_file_from_name_tree(root->right);
    }
}