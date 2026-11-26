#include <stdio.h>
#include <string.h>

struct MissingItem
{
    int id;
    int category;
    char ItemName[50];
    char details[100];
    char Name[50];
    char Pno[20];
};

void Items(const struct MissingItem list[], int items)
{
    FILE *fp = fopen("C:\\Users\\KLH\\items.txt", "w");
    if (fp == NULL)
    {
        printf("\n Error: Item Not saved!");
        return;
    }
    for (int i = 0; i < items; i++)
    {
        fprintf(fp, "%d\n", list[i].id);
        fprintf(fp, "%d\n", list[i].category);
        fprintf(fp, "%s\n", list[i].ItemName);
        fprintf(fp, "%s\n", list[i].details);
        fprintf(fp, "%s\n", list[i].Name);
        fprintf(fp, "%s\n", list[i].Pno);
    }
    fclose(fp);
}

int openItems(struct MissingItem list[])
{
    FILE *fp = fopen("C:\\Users\\KLH\\items.txt", "r");
    if (fp == NULL)
    {
        return 0; // in case file runned for the first time
    }
    int count = 0;
    while (count < 500 && fscanf(fp, "%d\n", &list[count].id) == 1)
    {
        fscanf(fp, "%d\n", &list[count].category);
        fgets(list[count].ItemName, sizeof(list[count].ItemName), fp);
        list[count].ItemName[strcspn(list[count].ItemName, "\r\n")] = '\0';
        fgets(list[count].details, sizeof(list[count].details), fp);
        list[count].details[strcspn(list[count].details, "\r\n")] = '\0';
        fgets(list[count].Name, sizeof(list[count].Name), fp);
        list[count].Name[strcspn(list[count].Name, "\r\n")] = '\0';
        fgets(list[count].Pno, sizeof(list[count].Pno), fp);
        list[count].Pno[strcspn(list[count].Pno, "\r\n")] = '\0';

        count++;
    }
    fclose(fp);
    return count;
}

void Found(struct MissingItem list[], int *items, const char userName[], const char userPno[])
{
    if (*items >= 500)
    {
        printf("\nStorage is Full Please try Again Later");
        return;
    }
    int i = *items; // index for next available slot

    // Generate a unique ID starting from 101
    int nextId = 101;
    for (int j = 0; j < *items; j++)
    {
        if (list[j].id >= nextId)
        {
            nextId = list[j].id + 1;
        }
    }
    list[i].id = nextId;

    printf("\n\n\t\t/===============Submitting Missing Item===============/\n\n");
    printf("Total Stored Items:%d\n", *items);
    do
    {
        printf("Please Select the category of the missing item:\n1.Electronics And Gadgets\n2.Wallets or Cash\n3.Id or Documents\n4.Accessories or Jewelry\n5.Other Items\n");
        if (scanf("%d", &list[i].category) != 1)
        {
            while (getchar() != '\n')
                ;                 // clears invalid text input
            list[i].category = 0; // forces the loop to repeat
        }
        else
        {
            getchar(); // clears new line
        }
        if (list[i].category < 1 || list[i].category > 5)
        {
            printf("\nWrong Input!! Please enter a number between 1 and 5.\n\n");
        }
    } while (list[i].category < 1 || list[i].category > 5);

    printf("\nEnter the Items Name:");
    fgets(list[i].ItemName, sizeof(list[i].ItemName), stdin);
    list[i].ItemName[strcspn(list[i].ItemName, "\n")] = '\0';
    printf("\nEnter Further Details(eg:Color,where found,etc):\n");
    fgets(list[i].details, sizeof(list[i].details), stdin);
    list[i].details[strcspn(list[i].details, "\n")] = '\0';
    strcpy(list[i].Name, userName);
    strcpy(list[i].Pno, userPno);
    (*items)++;

    Items(list, *items);
    printf("\nItem Saved Successfully!! (Item ID: %d)", list[i].id);
}

// Note: 'const' is removed from list[] and 'items' is now a pointer so Search() can remove claimed items
void Search(struct MissingItem list[], int *items)
{
    printf("\n\n\t\t/===============Searching For Items===============/\n\n");
    if (*items == 0)
    {
        printf("\nNo Items have been Reported yet!");
        return;
    }
    int cat;
    do
    {
        printf("Select Category of the Missing Item:\n1.Electronics And Gadgets\n2.Wallets or Cash\n3.Id or Documents\n4.Accessories or Jewelry\n5.Other Items\n");
        if (scanf("%d", &cat) != 1)
        {
            while (getchar() != '\n')
            {
            }
            cat = 0; // forces the loop to repeat
        }
        else
        {
            getchar(); // clears new line
        }

        if (cat < 1 || cat > 5)
        {
            printf("\nWrong Input!! Please enter a number between 1 and 5.\n\n");
        }
    } while (cat < 1 || cat > 5);

    int found = 0;
    for (int i = 0; i < *items; i++)
    {
        if (list[i].category == cat)
        {
            printf("\nItem ID: %d", list[i].id);
            printf("\nItem Name: %s", list[i].ItemName);
            printf("\nDetails: %s", list[i].details);
            printf("\nReported By: %s", list[i].Name);
            printf("\nPhone Number of The Reporter: %s", list[i].Pno);
            printf("\n===================================\n");
            found = 1;
        }
    }
    if (!found)
    {
        printf("\nNo Matching Items Found!!");
        return;
    }

    // Ask if the user found their item so it can be removed immediately
    int claimId;
    printf("\nDid you find your item? Enter its Item ID to claim & remove it (or 0 to go back): ");
    if (scanf("%d", &claimId) != 1)
    {
        while (getchar() != '\n')
        {
        }
        printf("\nInvalid input! Returning to menu.");
        return;
    }
    getchar(); // clears newline

    if (claimId == 0)
    {
        return;
    }

    int removeIndex = -1;
    for (int i = 0; i < *items; i++)
    {
        if (list[i].id == claimId && list[i].category == cat)
        {
            removeIndex = i;
            break;
        }
    }

    if (removeIndex == -1)
    {
        printf("\nInvalid Item ID for this category! Nothing was removed.");
        return;
    }

    // Shift remaining items left to fill the removed slot
    for (int i = removeIndex; i < *items - 1; i++)
    {
        list[i] = list[i + 1];
    }

    (*items)--;
    Items(list, *items); // Save updated list back to items.txt
    printf("\nItem ID %d has been claimed and removed from the list!!", claimId);
}

int main()
{
    char Name[50];
    char Pno[20]; // so the phone number can store +92 and 0 before the number else it will write 0333 as 333
    int c;
    struct MissingItem items[500];
    int count = openItems(items);
    printf("Loaded %d Saved Items from C:\\Users\\KLH\\items.txt\n\n", count);
    printf("Please Enter your  Full name:\n");
    fgets(Name, sizeof(Name), stdin);
    Name[strcspn(Name, "\n")] = '\0';
    printf("Enter your Phone Number:\n");
    fgets(Pno, sizeof(Pno), stdin);
    Pno[strcspn(Pno, "\n")] = '\0';
    printf("\n\n\t\t/===============FINDIT===============/\n\n");
    printf("Welcome %s to Findit, where you can find everything easily and efficiently!", Name);
    while (1)
    {
        printf("\n\n Please Select ONE of the Options below(type 1 for 1st option and 2 for second and so on):\n");
        printf("1.Find a Missing Item\n2.Submit a Missing Item\n3.Exit\n");
        if (scanf("%d", &c) != 1)
        {
            while (getchar() != '\n')
            {
            }
            c = 0;
        }
        else
        {
            while (getchar() != '\n')
            {
            }
        }
        switch (c)
        {
        case 1:
            Search(items, &count); // Passes &count so Search can reduce count if an item is claimed
            break;
        case 2:
            Found(items, &count, Name, Pno);
            break;
        case 3:
            printf("\nI hope our App helped you %s\n\tThank you for Using!!", Name);
            return 0;
        default:
            printf("Wrong Input!! Please enter 1 , 2 , 3");
        }
    }
}