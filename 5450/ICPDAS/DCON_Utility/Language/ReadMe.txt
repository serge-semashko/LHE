-----------------Multiple Language ReadMe-----------------------------------------------

1. The default language is English. 
  	If there are no any files in the language folder, use default English language 
	DCON utility will use the "last quit using language" when program executes

2. Add Language:Copy English.ini to xxx.ini file in the language folder
	       (xxx is the created language name which you can select it on Utility menu).

3. Edit .ini file: A ini file have 2 part : haed and index , 

	Ex:	[Main_Form]     		 ->dead
		Menu_File=File			 ->index
		.         
		.
		.

		[DataLogger]			->head
		Frame_Logger=Data logger	->index
		Label_Config=Log Config :	->index
		.	     ~~~~~~~~~~~~
		.            fix those words to your language words
		.
	
	(It only needs to fix those words that located the "=" right
	 Please refer to the difference between English.ini and Traditional_Chinese.ini)

4. Change language:Select your language on the "language" menu option in the DCON Utility