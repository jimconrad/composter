/****************************************************************
 * MOSTask --- Header for the mos model for a task (process)
 * 
 ***************************************************************/
 class MOSTask {

  public:
    MOSTask(byte,int,void *(void));

  private:
    byte priority;
    byte state;
 };

