#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<pthread.h>
#include	<unistd.h>
#include	<string>


//---			Definition of constants:			---//
 
//  PURPOSE:  To tell how many flowers each bee hive must visit.
const int	NUM_FLOWERS_TO_COLLECT	= 5;

//  PURPOSE:  To tell the number of bee hives that exist.
const int	NUM_BEE_HIVES	= 4;

//  PURPOSE:  To hold the names of the flowers:
const char*   	FLOWER_NAME_ARRAY[]
				= { "Jasmine",
				    "Daffodil",
				    "Daisy",
				    "Stinging Nettle",
				    "Venus fly trap",
				    "Tumbleweed",
				    "Kudzu",
				    "Poison Ivy"
				  };

//  PURPOSE:  To tell how many flower names there are.
const size_t	NUM_FLOWER_NAMES= sizeof(FLOWER_NAME_ARRAY)/sizeof(const char*);


//---		    Definition of classes and structs:			---//

//  PURPOSE:  To represent a flower.
class		Flower
{
  //  I.  Member vars:
  //  PURPOSE:  To hold address of the name of the flower as a C-string.
  const char*			nameCPtr_;

  //  PURPOSE:  To hold the address of the Flower instance after '*this' one,
  //	or 'NULL' if there is no such Flower.
  Flower*			nextPtr_;

  //  II.  Disallowed auto-generated methods:
  //  No copy constructor:
  Flower			(const Flower&);

  //  No copy assignment op:
  Flower&	operator=	(const Flower&);

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To make '*this' a stand-alone Flower instance with a randomly-
  //  	chosen name.  No parameters.  No return value.
  Flower			() :
				nameCPtr_(FLOWER_NAME_ARRAY[rand() %
					  NUM_FLOWER_NAMES]
					 ),
				nextPtr_(NULL)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No
  //	return value.
  ~Flower			()
				{ }

  //  V.  Accessors:
  //  PURPOSE:  To return the name of the flower.  No parameters.
  const char*	getNameCPtr	()
				const
				{ return(nameCPtr_); }

  //  PURPOSE:  To return the address of the Flower instance after '*this' one,
  //	or 'NULL' if there is no such Flower.
  Flower*	getNextPtr	()
				const
				{ return(nextPtr_); }

  //  VI.  Mutators:
  //  PURPOSE:  To note that the next flower in the list has address
  //	'newNextPtr'.  No return value.
  void		setNextPtr	(Flower*	newNextPtr
  				)
				{ nextPtr_ = newNextPtr; }

  //  VII.  Methods that do main and misc work of class:

};

class		PrivateGarden
{
  //  I.  Member vars:
  //  YOUR MEMBER VARS HERE
  //***********PrivateGarden mem vars (C1)***********//
  Flower* startPtr;
  Flower* endPtr;
  int nodesInList;

  //  II.  Disallowed auto-created methods:
  //  No copy constructor:
  PrivateGarden			(const PrivateGarden&);

  //  No copy assignment op:
  PrivateGarden&	operator=	(const PrivateGarden&);

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to an empty garden.  No parameters.
  //	No return value.
  PrivateGarden			()
				{
//**********Initialize (C2)***********//
				  startPtr = NULL;
          endPtr = NULL;
          nodesInList = 0;
				}

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //  	No return value.
  ~PrivateGarden			()
				{
//**********Delete list (C3)***********//
          Flower* temp;
          while(startPtr != NULL){
            temp = startPtr;
            startPtr = startPtr->getNextPtr();
            nodesInList--;
            delete(temp);
          }
				}

  //  V.  Accessor(s):
  //  PURPOSE:  To hold length of '*this' list.
  int		getNumFlowers	()
  				const
//**********returns count of nodes (C4)***********//
				{ return(nodesInList); /* CHANGE THAT 0 */}


  //  VI.  Mutator(s):

  //  VII.  Methods that do main and misc. work of class:
  //  PURPOSE:  To add the Flower with address 'flowerPtr' at the back of
  //	'*this' PrivateGarden of Flower instances.  No return value.
  void		store		(Flower*	flowerPtr
				)
				{
//**********creates list (C5)***********//
          Flower* nodePtr = new Flower();
          if (startPtr == NULL){
            startPtr = nodePtr;
          }else{
            endPtr->setNextPtr(nodePtr);
          }
          endPtr = nodePtr;
				  nodesInList++;
				}

  //  PURPOSE:  To print this list of Flower instances in '*this' PrivateGarden.
  //	No parameters.  No return value.
  void		print		()
				{
//**********Prints list (C6)***********//
          const Flower* run;
			    for (run = startPtr; run != NULL; run = run->getNextPtr()){
            printf("%s\t\n", run->getNameCPtr());
          } 
          
				}
 
};


struct		Hive
{
  std::string			name_;
  PrivateGarden*		gardenPtr_;

  Hive				() :
  				name_(""),
				gardenPtr_(NULL)
				{ }

  ~Hive				()
  				{
				  delete(gardenPtr_);
				}

  const char*	getNameCPtr	()
  				const
				{ return(name_.c_str()); }

};


//---			Definition of global vars:			---//

//  PURPOSE:  To hold the address of the flower offered by the farmer.
//	or to hold 'NULL' if there is no such Flower.
Flower*		availableFlowerPtr	= NULL;

//  PURPOSE:  To tell how much honey has been produced by the bee hives.
int		honey			= 0;

//  YOUR CODE HERE to add global vars to control access to availableFlowerPtr and honey:
pthread_mutex_t flwrLock;
pthread_cond_t isFull;
pthread_cond_t isEmpty;
pthread_mutex_t honeyLock;

//---			Definition of main functions:			---//

//  PURPOSE:  To be the function run by the bee hive threads.  'vPtr' points
//	to an instance of 'Hive'.  Returns 'NULL'.
void*		hive		(void*		vPtr
				)
{
  //  I.  Application validity check:

  //  II.  Get the flowers:
  //  II.A.  Initialize local vars:
  //**********Declare vars (D2.A)***********//
  Hive*		hivePtr		= (Hive*)vPtr;  // CHANGE THAT NULL
  PrivateGarden* gardenPtr	= hivePtr->gardenPtr_;  // CHANGE THAT NULL

  //  II.B.  Each iteration obtains another Flower instance for the graden
  //  	     of Hive '*hivePtr':
  while  (gardenPtr->getNumFlowers() < NUM_FLOWERS_TO_COLLECT)
  {
    //  YOUR CODE HERE: Make access to avaialbleFlowerPtr thread-safe
    pthread_mutex_lock(&flwrLock);
    while  (availableFlowerPtr == NULL)
    {
      printf("%s: \"Hey!  No flowers, no honey!\"\n",hivePtr->getNameCPtr());
      pthread_cond_wait(&isEmpty, &flwrLock);
    }

    printf("%s: \"A %s!  Sure we will take that!\"\n",
	   hivePtr->getNameCPtr(),availableFlowerPtr->getNameCPtr()
	  );
    gardenPtr->store(availableFlowerPtr);
    availableFlowerPtr	= NULL;
    pthread_cond_signal(&isFull);
    pthread_mutex_unlock(&flwrLock);

    //  Leave this outside critical section:
    sleep(rand() % 3);	// Please leave this OUT of the critical section
  }

  //  II.C.  Add to the honey when have enough flowers:
  printf("%s \"Finally, enough flowers to make some honey.\"\n",
  	 hivePtr->getNameCPtr()
	);

  //  YOUR CODE HERE: Make incrementing honey thread-save
  pthread_mutex_lock(&honeyLock);
  honey++;
  pthread_mutex_unlock(&honeyLock);
  //  III.  Finished:
  return(NULL);
}


//  PURPOSE:  To be the function run by the farmer thread.  'vPtr' is ignored.
//	Returns 'NULL'.
void*		farmer		(void*	vPtr)
{
  //  I.  Application validity check:

  //  II.  Give flowers:
  //  II.A.  Each iteration creates and gives another Flower instance
  //  	     until there is sufficient honey:
  while  (true)
  {
    //  YOUR CODE HERE: Make access to honey thread-safe
    pthread_mutex_lock(&honeyLock);
    if  (honey >= NUM_BEE_HIVES)
    {
      pthread_mutex_unlock(&honeyLock);
      break;
    }
    pthread_mutex_unlock(&honeyLock);
    printf("Farmer: \"I have to gather *more* flowers?!?\"\n");

    //  YOUR CODE HERE: Make access to availableFlowerPtr thread-safe
    pthread_mutex_lock(&flwrLock);
    while  (availableFlowerPtr != NULL)
    {
      printf("Farmer: \"Hey, you said you wanted"
      	     " a flower, come and take it.\"\n"
	    );
      pthread_cond_wait(&isFull, &flwrLock);
    }

    availableFlowerPtr	= new Flower;
    printf("Farmer: \"Okay here is another flower: a %s\"\n",
	   availableFlowerPtr->getNameCPtr()
	  );

    pthread_cond_signal(&isEmpty);
    pthread_mutex_unlock(&flwrLock);
    //  Leave this outside critical section:
    sleep(rand() % 3);	// Please leave this OUT of the critical section
  }

  //  III.  Finished:
  printf("Farmer \"I *finally* got my honey!\"\n");
  return(NULL);
}


//  PURPOSE:  To run the program.  Ignores command line arguments.  Returns
//	'EXIT_SUCCESS' to OS.
int		main		()
{
  //  I.  Application validity check:

  //  II.  Have the farmer give Flower instances until sufficient honey
  //  	   has been obtained:
  //  II.A.  Randomize random number generator:
  srand(getpid());

  //  II.B.  Initialize global vars:
  Hive		hiveArray[NUM_BEE_HIVES];
//**********Declare vars (D1.A)***********//
  pthread_t farmerThread;
  pthread_t beeHives[NUM_BEE_HIVES];
  //  Add something here?
  pthread_mutex_init(&flwrLock, NULL);
  pthread_mutex_init(&honeyLock, NULL);
  pthread_cond_init(&isFull, NULL);
  pthread_cond_init(&isEmpty, NULL);
//**********creates threads (D1.B)***********//
  //  II.C.  Make threads:
  //  II.C.1.  Make bee hive threads:
  for  (int i = 0;  i < NUM_BEE_HIVES;  i++)
  {
    hiveArray[i].name_		= std::string("Hive ") + (char)('A'+i);
    hiveArray[i].gardenPtr_	= new PrivateGarden;
    pthread_create(&beeHives[i],NULL,hive,(void*)&hiveArray[i]);
  }

  //  II.C.2.  Make farmer thread:
  //  Add something here?
  //**********creates threads (D1.B)***********//
  pthread_create(&farmerThread,NULL,farmer,NULL);

  //  II.D.  Wait for child threads:
  //  II.D.1.  Wait for bee hive threads:
  for  (int i = 0;  i < NUM_BEE_HIVES;  i++)
  {
//**********wait for threads (D1.C)***********//
    //  Add something here?
    pthread_join(beeHives[i], NULL);
    printf("%s has:\n",hiveArray[i].getNameCPtr());
    hiveArray[i].gardenPtr_->print();
  }

  //  II.D.2.  Wait for farmer thread:
  //**********wait for threads (D1.C)***********//
  pthread_join(farmerThread, NULL);

  //  II.E.  Get rid of global vars:
  pthread_mutex_destroy(&flwrLock);
  pthread_mutex_destroy(&honeyLock);
  pthread_cond_destroy(&isFull);
  pthread_cond_destroy(&isEmpty);
  //  Add something here?

  //  III.  Finished:
  return(EXIT_SUCCESS);
}