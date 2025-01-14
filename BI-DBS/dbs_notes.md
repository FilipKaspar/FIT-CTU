# <p style="text-align: center"><b>DBS</b></p>

**Entities** - Group of atributes. Can be either **Strong** or **Weak**  
**Atributes** - Multiple types. Information that coresponds to the whole entity.
**Relations** - Not mandatory. For better understanding between 2 different entities.

![entity](images/entity.jpg)

### Types of Atributes
1. **Mandatory_unique**  
2. **Mandatory**  
3. **Optional**  

![atributes](images/atributes.jpg)

### How we differentiate types of relations

**Cardinality** - Describes how many unique atributes in a entity can be assign to a different entity

1.**Cardinality 1:1** - Entity #1 has only one atribute from entity #2 and vice versa

![k1_1](images/k1_1.jpg)

2.**Cardinality 1:N** - Entity #1 can have more atributes from entity #2, but not the other way around

![k1_N](images/k1_N.jpg)

3.**Cardinality M:N** - Entity #1 can have more atributes from entity #2 and vice versa

![kM_N](images/kM_N.jpg)

### Participation
1. **Mandotory part** - Solid line or "1"
2. **Optional part** - Dashed or "0"

Signs **<** and  **>** are being used for determing if a entity in a relation has to have exactly one atribute from second entity or not. If **NO** the **arrows** are added to the line 

![mandatory](images/mandatory.jpg)  
Cinema has to have at least on movie  
Certain movie is being played at and only at one cinema 

![participation](images/participation.jpg)  
Cinema can have zero movies or more  
Certain movie has to be played at only one cinema

### Weak Entity

**Weak Entity** - Can exist only if certain entity is connected to this entity. Sign: "**|**"

![weak](images/weak.jpg)

In this example entity **Room** can't exist if entity **Block** doesn't exist

### Recursive relation

Used for describing the relation inside of the same entity.  
Ex. Someone is a boss of a pesron in the same entity.

![recursive](images/rescursive.jpg)

### Example of almost completed database

![example](images/ex_1.jpg)

Add description  - how the schema actually works

### Relation algebra

**NF - Normal Form**

Types to differentiate SQL languages for different data handling

![types](images/types.JPG)

**Rules of Relational databases**

![rules](images/rules.JPG)

**Terminology**

![terminology](images/terminology.JPG) 

**Definitions**

![def1](images/def1.JPG)
![def2](images/def2.JPG)

**Integrity restrictions**

![io](images/ir.JPG)

Cinema can play the movie only one time

IO3 and IO4 we can't describe on the base  of relation model  
IO3: for example so called trigger to trigger if statement if movie is supposed to be played for 3rd time. That's how we go around it

**Overall**

![overall](images/overall.JPG)

**Select and Project**

![sp](images/sp.JPG)

**Join and Rename**

![join_rename](images/join_rename.JPG)

**General Join**

![ojoin](images/ojoin.JPG)

**Operations**

![operations](images/operations.JPG)

**Join example**

![join_ex](images/ex.JPG)

### General half-join

![halfjoin](images/halfjoin.JPG)

### Antijoin

![antijoin](images/antijoin.JPG)

### Summary

![summary](images/summary.JPG)

### SQL Data Definition Language
**Create, alter and drop TABLE**

![create](images/create.JPG)
![alter_drop](images/alter_srop.JPG)

### Integrity Restrictions in SQL : IR

![io](images/io.JPG)

### Data types in SQL

![data](images/data.JPG)

### How is conceptual schema being translated to relation

![trans](images/trans.JPG)
![steps](images/steps.JPG)

**Every cardinality and participation connection, weak and strong entity, loop, ISA changes how is the specific entity being translated**
