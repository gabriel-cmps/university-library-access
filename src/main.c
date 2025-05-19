#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const char WEEK_DAYS[7][10] = {
    "Domingo", "2ª-feira", "3ª-feira", "4ª-feira", "5ª-feira", "6ª-feira", "Sabado"};

typedef enum
{
  Sunday = 0,
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday
} WeekDay;

typedef enum
{
  Undergraduate = 1,
  Postgraduate,
  Professor,
  Visitor
} UserCategory;

typedef struct
{
  char user_id[24];
  int age;
  char course[50];
  UserCategory user_category;
  int has_library_debt;
  char access_day[10];
  char access_time[10];
} UserAccess;

void clear_input_buffer()
{
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
  if (ch == EOF)
  {
    clearerr(stdin);
  }
}

bool is_valid_time(const char *access_time)
{
  int hour = 0, min = 0;
  char h = 0, extra = 0;

  // "Hh" (ex: 8h, 01h, 23h)
  if (sscanf(access_time, "%2d%c%c", &hour, &h, &extra) == 2 && h == 'h' && access_time[0] != '\0')
  {
    if (hour >= 0 && hour <= 23)
      return true;
  }
  // "HhMM" (ex: 8h30, 01h05, 23h59, 0h00)
  if (sscanf(access_time, "%2d%c%2d%c", &hour, &h, &min, &extra) == 3 && h == 'h' && access_time[0] != '\0')
  {
    if (strlen(access_time) == 5 && hour >= 0 && hour <= 23 && min >= 0 && min <= 59)
      return true;
  }
  return false;
}

bool is_valid_day(const char *access_day)
{
  int total_week_days = sizeof(WEEK_DAYS) / sizeof(WEEK_DAYS[0]);
  for (int i = 0; i < total_week_days; i++)
  {
    if (strcmp(access_day, WEEK_DAYS[i]) == 0)
    {
      return true;
    }
  }
  return false;
}

bool is_valid_user_category(int user_category)
{
  return (user_category >= Undergraduate && user_category <= Visitor);
}

bool is_time_in_range(const char *access_time, int start_hour, int end_hour)
{
  int hour, min = 0;
  char extra;
  if (sscanf(access_time, "%2dh%2d%c", &hour, &min, &extra) >= 1)
  {
    int total = hour * 60 + min;
    int start = start_hour * 60;
    int end = end_hour * 60;
    return total >= start && total <= end;
  }
  return false;
}

bool is_day_in_range(const char *access_day, int start_day, int end_day)
{
  int total_week_days = sizeof(WEEK_DAYS) / sizeof(WEEK_DAYS[0]);
  for (int i = 0; i < total_week_days; i++)
  {
    if (strcmp(access_day, WEEK_DAYS[i]) == 0)
    {
      return (i >= start_day && i <= end_day);
    }
  }
  return false;
}

void print_debt_denied()
{
  printf("Resultado: Acesso negado!\n");
  printf("Motivo: Usuários dessa categoria só podem acessar sem débitos.\n");
}

void print_time_day_denied(int start_hour, int end_hour, const char *start_day, const char *end_day)
{
  printf("Resultado: Acesso negado!\n");
  printf("Motivo: Acesso permitido somente das %dh às %dh, de %s a %s.\n",
         start_hour, end_hour, start_day, end_day);
}

void print_access_granted()
{
  printf("Resultado: Acesso permitido à biblioteca.\n");
}

void process_undergraduate(const UserAccess *user)
{
  if (user->has_library_debt)
  {
    print_debt_denied();
    return;
  }
  if (!is_time_in_range(user->access_time, 8, 22) ||
      !is_day_in_range(user->access_day, Monday, Friday))
  {
    print_time_day_denied(8, 22, "segunda-feira", "sexta-feira");
    return;
  }
  print_access_granted();
}

void process_postgraduate(const UserAccess *user)
{
  if (user->has_library_debt)
  {
    print_debt_denied();
    return;
  }
  if (!is_time_in_range(user->access_time, 8, 22) ||
      !is_day_in_range(user->access_day, Monday, Saturday))
  {
    print_time_day_denied(8, 22, "segunda-feira", "sábado");
    return;
  }
  print_access_granted();
}

void process_professor(const UserAccess *user)
{
  print_access_granted();
}

void process_visitor(const UserAccess *user)
{
  if (user->has_library_debt)
  {
    print_debt_denied();
    return;
  }
  if (!is_time_in_range(user->access_time, 9, 17) ||
      !is_day_in_range(user->access_day, Monday, Friday))
  {
    print_time_day_denied(9, 17, "segunda-feira", "sexta-feira");
    return;
  }
  print_access_granted();
}

int main()
{
  UserAccess user;

  do
  {
    printf("Digite a matrícula do usuário: ");
    int result = scanf("%s", user.user_id);
    if (result != 1 || strlen(user.user_id) == 0)
    {
      clear_input_buffer();
      printf("Resultado: Entrada Inválida!\n");
      printf("Motivo: Matrícula não pode ser vazia. Por favor, digite novamente.\n");
      user.user_id[0] = '\0';
    }
  } while (strlen(user.user_id) == 0);

  do
  {
    printf("Digite a idade: ");
    char extra;
    int result = scanf("%d%c", &user.age, &extra);
    if (result != 2 || extra != '\n')
    {
      clear_input_buffer();
      printf("Resultado: Entrada inválida!\n");
      printf("Motivo: Você deve digitar um número inteiro.\n");
      user.age = 0;
    }
    else if (user.age < 18)
    {
      printf("Resultado: Idade insuficiente!\n");
      printf("Motivo: A idade mínima permitida é 18 anos.\n");
      user.age = 0;
    }
  } while (user.age < 18);

  do
  {
    printf("Digite o curso: ");
    int result = scanf(" %[^\n]", user.course);
    if (result != 1 || strlen(user.course) == 0)
    {
      clear_input_buffer();
      printf("Resultado: Entrada Inválida!\n");
      printf("Motivo: O nome do curso não pode ser vazio. Por favor, digite novamente.\n");
      user.course[0] = '\0';
    }
  } while (strlen(user.course) == 0);

  do
  {
    printf("Selecione a categoria do usuário:\n");
    printf(" 1 - Aluno de Graduação\n");
    printf(" 2 - Aluno de Pós-graduação\n");
    printf(" 3 - Professor\n");
    printf(" 4 - Visitante\n");
    printf("Opção: ");
    char extra;
    int result = scanf("%d%c", (int *)&user.user_category, &extra);

    if (result != 2 || extra != '\n')
    {
      clear_input_buffer();
      user.user_category = 0;
    }
    else if (!is_valid_user_category(user.user_category))
    {
      user.user_category = 0;
    }

    if (result != 2 || extra != '\n' || !is_valid_user_category(user.user_category))
    {
      printf("Resultado: Entrada Inválida!\n");
      printf("Motivo: Categoria inexistente. Por favor, escolha uma opção válida (1 a 4).\n");
    }

  } while (!is_valid_user_category(user.user_category));

  do
  {
    printf("Está em débito com a biblioteca? (1 = Sim, 0 = Não): ");
    char extra;
    int result = scanf("%d%c", &user.has_library_debt, &extra);
    if (result != 2 || extra != '\n')
    {
      clear_input_buffer();
      user.has_library_debt = -1;
    }
    else if (user.has_library_debt != 0 && user.has_library_debt != 1)
    {
      user.has_library_debt = -1;
    }

    if (result != 2 || extra != '\n' || user.has_library_debt != 0 && user.has_library_debt != 1)
    {
      printf("Resultado: Entrada Inválida!\n");
      printf("Motivo: Opção inválida. Digite 1 para Sim ou 0 para Não.\n");
    }
  } while (user.has_library_debt != 0 && user.has_library_debt != 1);

  do
  {
    printf("Digite o dia da tentativa de acesso (ex: 2ª-feira): ");
    int result = scanf(" %[^\n]", user.access_day);
    if (result != 1 || !is_valid_day(user.access_day))
    {
      clear_input_buffer();
      printf("Resultado: Entrada Inválida!\n");
      printf("Motivo: Dia da semana inválido. Por favor, digite conforme o exemplo (ex: 2ª-feira).\n");
      user.access_day[0] = '\0';
    }
  } while (!is_valid_day(user.access_day));

  do
  {
    printf("Digite o horário da tentativa de acesso (ex: 20h): ");
    int result = scanf("%s", user.access_time);
    if (result != 1 || !is_valid_time(user.access_time))
    {
      clear_input_buffer();
      printf("Resultado: Entrada Inválida!\n");
      printf("Motivo: Horário inválido. Use o formato correto, por exemplo: 20h ou 20h30.\n");
      user.access_time[0] = '\0';
    }
  } while (!is_valid_time(user.access_time));

  switch (user.user_category)
  {
  case Undergraduate:
    process_undergraduate(&user);
    break;
  case Postgraduate:
    process_postgraduate(&user);
    break;
  case Professor:
    process_professor(&user);
    break;
  case Visitor:
    process_visitor(&user);
    break;
  }

  return 0;
}
